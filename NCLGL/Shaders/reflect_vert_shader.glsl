#version 420 core

uniform mat4 modelMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec4 colour;

out Vertex 
{
	vec2 texCoord;
	vec3 normal;
	vec4 colour;
	vec3 tangent;
} OUT;

void main(void)	
{
	OUT.texCoord = texCoord;
	OUT.normal = normal;
	OUT.tangent = tangent;
	OUT.colour = colour;
	gl_Position	= vec4(position, 1.0);
}