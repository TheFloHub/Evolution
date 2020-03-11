#version 330 core										
layout (location = 0) out vec4 gNormal;					
layout (location = 1) out vec4 gAlbedo;					
														
in vec3 Normal;											
in vec2 TexCoord;										
														
uniform vec3 diffuseColor;								
uniform sampler2D diffuseTexture;						
														
void main()																		
{																				
	gNormal.xyz = (normalize(Normal)+1.0f)/2.0f;								
	gNormal.a = 1.0f;															
	gAlbedo.rgb = diffuseColor*texture(diffuseTexture, TexCoord).rgb;			
	gAlbedo.a = 0.0f;															
}