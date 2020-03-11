#version 330 core													
layout (location = 0) in vec3 position;								
layout (location = 1) in vec3 normal;								
																	
uniform mat4 modelViewProjectionMatrix;								
uniform mat3 normalMatrix;										
																	
out vec3 Normal;												
																	
void main()															
{																	
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0f);	
	Normal = normalMatrix * normal;									
}