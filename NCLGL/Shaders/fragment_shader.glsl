#version 450 core

uniform sampler2D diffuseTex;
uniform int font;
uniform int sun;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;
out vec4 FragColour;
void main(void)	
{
	if(font == 1 || sun == 1)
	{
		FragColour = texture(diffuseTex, IN.texCoord);
	}
	else
	{
		FragColour = vec4(1.0);
	}
}