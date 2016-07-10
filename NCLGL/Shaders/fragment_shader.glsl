/*
Author: Shaun Heald
Basic fragment shader, either samples from a texture or reads from colour input variable and sets output colour. 
*/

#version 420 core

uniform sampler2D diffuseTex;
uniform int usingTexture;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 FragColour;

void main(void)	
{
	if(usingTexture == 1)
	{
		FragColour = texture(diffuseTex, IN.texCoord);
	}
	else
	{
		FragColour = vec4(IN.colour);
	}
}