#version 330 core						
										
out vec4 FragColor;						
in vec2 TexCoords;						
in vec3 Ray;							
										
uniform sampler2D gDepth;				
uniform sampler2D gNormal;				
uniform sampler2D gAlbedo;	
uniform sampler2D gShadowMap;				
										
uniform vec3 lightPosDir;				
uniform vec3 lightColor;				
uniform float lightIntensity;			
uniform int lightType;					
										
uniform float nearPlane;				
uniform float farPlane;		

uniform mat4 cameraToLightTrafo;				
										
float LinearizeDepth(float depth)																	
{																									
	float z = depth * 2.0 - 1.0; // back to NDC 													
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));		
}			

float ShadowCalculation(vec3 fragPosCameraSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = (cameraToLightTrafo*vec4(fragPosCameraSpace, 1.0f)).xyz;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001); 
	currentDepth -= bias;
	
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float lightDepth1 = textureOffset(gShadowMap, projCoords.xy, ivec2(-1,  0)).r; 
    float lightDepth2 = 	  texture(gShadowMap, projCoords.xy).r; 
	float lightDepth3 = textureOffset(gShadowMap, projCoords.xy, ivec2( 1,  0)).r; 
	
	float lightDepth4 = textureOffset(gShadowMap, projCoords.xy, ivec2(-1, -1)).r; 
	float lightDepth5 = textureOffset(gShadowMap, projCoords.xy, ivec2( 0, -1)).r; 
	float lightDepth6 = textureOffset(gShadowMap, projCoords.xy, ivec2( 1, -1)).r; 
	
	float lightDepth7 = textureOffset(gShadowMap, projCoords.xy, ivec2(-1,  1)).r; 
	float lightDepth8 = textureOffset(gShadowMap, projCoords.xy, ivec2( 0,  1)).r; 
	float lightDepth9 = textureOffset(gShadowMap, projCoords.xy, ivec2( 1,  1)).r; 
	
    // Check whether current frag pos is in shadow
    float shadow = 0.0;
	shadow += currentDepth > lightDepth1 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth2 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth3 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth4 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth5 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth6 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth7 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth8 ? 1.0 : 0.0;
	shadow += currentDepth > lightDepth9 ? 1.0 : 0.0;
	shadow /= 9.0;
	shadow *= 0.75;
	
	if (projCoords.z > 1.0)
	{
        shadow = 0.0;
	}

    return shadow;
}																						
																									
void main()																							
{																									
	// retrieve data from gbuffer																	
	float depth = -LinearizeDepth(texture(gDepth, TexCoords).r);									
	vec3 normal = 2.0f*texture(gNormal, TexCoords).rgb -1.0f;										
	vec4 baseDiffuseColor = texture(gAlbedo, TexCoords);											
	vec3 baseSpecularColor = vec3(1.0f, 1.0f, 1.0f);												
	float shininess = baseDiffuseColor.a *255.0f;													
																									
	// position from linear depth																	
	vec3 nRay = normalize(Ray);																		
	vec3 vertexPosition = vec3(0.0f, 0.0f, depth);													
	vertexPosition.xy = nRay.xy * vertexPosition.z / nRay.z;										
																									
	vec3 lightDir = vec3(0.0f, 0.0f, 0.0f);															
	float attenuation = 1.0f;																		
																									
	// directional light																			
	if (lightType == 0)																				
	{																								
		lightDir = -lightPosDir; // light direction must come from object from now on  																	
	}																								
	// point light																					
	else 																							
	{																								
		lightDir = lightPosDir - vertexPosition;													
		attenuation = 1/dot(lightDir, lightDir);													
		lightDir = normalize(lightDir);																
	}																								
																									
	// Diffuse																						
	float diffuseIntensity = max(dot(normal, lightDir), 0.0f);										
 																									
	// Specular																						
	float specularIntensity = 0.0f;																	
	if (shininess != 0.0f)																			
	{																								
		vec3 viewDir = normalize(-vertexPosition);													
		//vec3 reflectedLightDir = reflect(-lightDir,normal);										
		//specularIntensity = pow(max(dot(viewDir, reflectedLightDir), 0.0f), shininess);			
		vec3 halfwayDir = normalize(lightDir + viewDir);											
		specularIntensity = pow(max(dot(normal, halfwayDir), 0.0f), shininess);						
	}																								
 	
	float shadow = ShadowCalculation(vertexPosition, normal, lightDir);
	vec3 result = (diffuseIntensity*baseDiffuseColor.rgb + specularIntensity*baseSpecularColor) * attenuation * lightColor * lightIntensity * (1.0f-shadow);		
																									
	FragColor = vec4(result, 1.0f);																	
}