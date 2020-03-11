#version 330 core												
layout (location = 0) in vec3 position;							
layout (location = 1) in vec3 normal;							
layout (location = 2) in vec2 texCoords;						
																
out vec2 TexCoords;												
out vec3 Ray;													
																
uniform mat4 inverseProjection;									
																
void main()														
{																
	gl_Position = vec4(position, 1.0f);							
	TexCoords = texCoords;										
	Ray = (inverseProjection * gl_Position).xyz;				
}