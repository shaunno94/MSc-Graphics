#version 420 core

uniform sampler2D heights;

in vec3 position;
in vec2 texCoord;

out Vertex 
{
	vec2 texCoord;
} OUT;

void main(void)	
{
	//Displace Y values
	vec4 texel = texture(heights, texCoord);
	
	OUT.texCoord = texCoord;
	gl_Position = vec4(position.x, (texel.r * 2500), position.z, 1.0);
}