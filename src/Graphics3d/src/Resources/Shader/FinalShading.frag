#version 330 core						
										
out vec4 FragColor;						
in vec2 TexCoords;						
										
uniform sampler2D inputTexture;		
// also albedo for ambient???									
																									
void main()																							
{																					
	// ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //vec3 ambient = vec3(0.03) * albedo;

    vec3 color = texture(inputTexture, TexCoords).rgb; //ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
	
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);																	
}