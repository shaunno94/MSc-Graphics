#version 450 core

uniform mat4 MVP;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void)	
{
	OUT.texCoord = texCoord;
	OUT.colour = colour;
	gl_Position	= MVP * vec4(position, 1.0);
}