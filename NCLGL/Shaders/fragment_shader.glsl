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
		FragColour = vec4(1.0);
	}
}