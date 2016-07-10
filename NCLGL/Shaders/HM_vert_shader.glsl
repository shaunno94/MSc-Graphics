/*
Author: Shaun Heald
Vertex shader for heightmap, samples a height texture and displaces Y accordingly.
*/

#version 420 core

uniform sampler2D heights;

in vec3 position;
in vec2 texCoord;

out Vertex 
{
	vec2 texCoord;
} OUT;

const float HEIGHT_SCALE = 2500.0f;

void main(void)	
{
	//Sample height texture.
	float height = texture(heights, texCoord).r;
	
	OUT.texCoord = texCoord;
	//Displace Y co-ordinate
	gl_Position = vec4(position.x, (height * HEIGHT_SCALE), position.z, 1.0);
}