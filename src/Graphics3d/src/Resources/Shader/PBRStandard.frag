#version 330 core										
layout (location = 0) out vec4 gNormal;					
layout (location = 1) out vec4 gAlbedo;					
														
in vec3 Normal;									
														
uniform vec3 albedo;								
uniform float metallic;
uniform float roughness;					
														
void main()																		
{																				
	gNormal.xyz = (normalize(Normal)+1.0f)/2.0f;								
	gNormal.a = roughness;															
	gAlbedo.rgb = albedo;			
	gAlbedo.a = metallic;															
}