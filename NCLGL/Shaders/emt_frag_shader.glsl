#version 450 core
uniform sampler2D diffuseTex;
in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;
out vec4 FragColour;

void main(void)	
{
	FragColour = texture(diffuseTex, IN.texCoord);
	FragColour.a = IN.colour.a;
}