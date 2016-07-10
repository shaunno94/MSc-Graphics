/*
Author: Shaun Heald
Simple vertex shader used for drawing a full screen quad with ortho projection.
*/

#version 420 core

uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)	
{
	gl_Position	= projMatrix * vec4(position, 1.0);
	OUT.texCoord = texCoord;
}