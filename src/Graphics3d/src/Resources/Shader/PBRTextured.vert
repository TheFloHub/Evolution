#version 330 core													
layout (location = 0) in vec3 position;								
layout (location = 1) in vec3 normal;								
layout (location = 2) in vec2 texCoord;	
layout (location = 3) in vec3 tangent;								
layout (location = 4) in vec3 bitangent;									
																	
uniform mat4 modelViewProjectionMatrix;								
uniform mat3 normalMatrix;											
uniform vec2 tiling;												
																	
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;													
out vec2 TexCoord;													
																	
void main()															
{																	
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0f);	
	Normal = normalMatrix * normal;		
	Tangent = normalMatrix * tangent;	
	Bitangent = normalMatrix * bitangent;		
	TexCoord = tiling*texCoord;										
}