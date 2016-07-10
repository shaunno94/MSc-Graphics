/*
Author: Shaun Heald
Simple vertex shader outputs position multiplied by MVP, outputs normal, colour, texture co-ordinate.
*/

#version 420 core

uniform mat4 MVP;

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;

out Vertex 
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
} OUT;

void main(void)	
{
	gl_Position = MVP * vec4(position, 1.0);
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normal = normal;
}