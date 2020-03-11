#version 330 core						
layout (location = 0) out vec3 color;		
					
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


const float PI = 3.14159265359;	
			
// ----------------------------------------------------------------------------			
float LinearizeDepth(float depth)																	
{																									
	float z = depth * 2.0 - 1.0; // back to NDC 													
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));		
}			
// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    //float a = roughness*roughness;
    float a2 = roughness*roughness;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
						
void main()																							
{																									
	// retrieve data from gbuffer								
	vec4 gNormalData = texture(gNormal, TexCoords).rgba;
	vec4 gAlbedoData = texture(gAlbedo, TexCoords).rgba;
	vec3 N = 2.0f * gNormalData.rgb - 1.0;	
	float roughness = gNormalData.a;	
	// TODO: correct gamma in albedo textures at tex3d upload
	vec3 albedo = vec3(pow(gAlbedoData.r, 2.2), pow(gAlbedoData.g, 2.2), pow(gAlbedoData.b, 2.2)); // gAlbedoData.rgb; 	or	  pow(gAlbedoData.rgb, 2.2) because albedo in sRGB space???		vec3(pow(gAlbedoData.r, 2.2), pow(gAlbedoData.g, 2.2), pow(gAlbedoData.b, 2.2));							
	float metallic = gAlbedoData.a;													
																									
	// position from linear depth										
	float depth = -LinearizeDepth(texture(gDepth, TexCoords).r);																	
	vec3 nRay = normalize(Ray);																		
	vec3 vertexPosition = vec3(0.0, 0.0, depth);													
	vertexPosition.xy = nRay.xy * vertexPosition.z / nRay.z;	
	vec3 V = normalize(-vertexPosition); // oder einfach -nRay???
	
	// light direction
	vec3 L = vec3(0.0, 0.0, 0.0);															
	float attenuation = 1.0;																																							
	if (lightType == 0) 	// directional light																			
	{																								
		L = -lightPosDir; 	// light direction must come from object from now on  																	
	}																																												
	else 					// point light																							
	{																								
		L = lightPosDir - vertexPosition;													
		attenuation = 1.0/dot(L, L);													
		L = normalize(L);																
	}	
	
	 // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	
	// calculate per-light radiance
	vec3 H = normalize(V + L);
	vec3 radiance = lightColor * lightIntensity * attenuation;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);   
	float G   = GeometrySmith(N, V, L, roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 nominator    = NDF * G * F; 
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	vec3 specular = nominator / denominator;

	// kS is equal to Fresnel
	vec3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	vec3 kD = vec3(1.0) - kS;
	// multiply kD by the inverse metalness such that only non-metals 
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - metallic;	  

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);        

	// add to outgoing radiance Lo
	color = (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	   
		
	// ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //vec3 ambient = vec3(0.03) * albedo;

    //vec3 color = ambient + Lo;

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    // gamma correct
    //color = pow(color, vec3(1.0/2.2)); 

    //FragColor = vec4(color, 1.0);															
}