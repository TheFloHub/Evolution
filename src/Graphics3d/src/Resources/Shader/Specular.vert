#version 330 core													
layout (location = 0) in vec3 position;								
layout (location = 1) in vec3 normal;								
layout (location = 2) in vec2 texCoord;								
																	
uniform mat4 modelViewProjectionMatrix;								
uniform mat3 normalMatrix;											
uniform vec2 tiling;												
																	
out vec3 Normal;													
out vec2 TexCoord;													
																	
void main()															
{																	
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0f);	
	Normal = normalMatrix * normal;									
	TexCoord = tiling*texCoord;										
}