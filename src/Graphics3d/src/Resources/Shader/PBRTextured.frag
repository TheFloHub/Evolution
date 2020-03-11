#version 330 core										
layout (location = 0) out vec4 gNormal;					
layout (location = 1) out vec4 gAlbedo;					
														
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;
in vec2 TexCoord;								
														
uniform sampler2D albedoTexture;	
uniform sampler2D metallicTexture;	
uniform sampler2D roughnessTexture;		
uniform sampler2D normalTexture;				
														
void main()																		
{					
	mat3 tbnmat = mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal));
	vec3 N = tbnmat * ((texture(normalTexture, TexCoord).xyz * 2.0 ) - 1.0);
	
															
	gNormal.xyz = (normalize(N) + 1.0) / 2.0;								
	gNormal.a = texture(roughnessTexture, TexCoord).r;															
	gAlbedo.rgb = texture(albedoTexture, TexCoord).rgb;			
	gAlbedo.a = texture(metallicTexture, TexCoord).r;										
}