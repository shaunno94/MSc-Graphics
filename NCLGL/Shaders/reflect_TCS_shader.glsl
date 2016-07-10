/*
Author: Shaun Heald
Lake node tessellation control shader.
Sets up the pipeline for tessellation, calculates the bi-normal, outputs data to the TES.
*/


#version 420 core

uniform mat4 modelMatrix;

layout(vertices = 4) out;

in Vertex {
	vec2 texCoord;
	vec3 normal;
	vec4 colour;
	vec3 tangent;

} IN[];

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec4 colour;
} OUT[];

void main(void)	
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 100;
		gl_TessLevelInner[1] = 100;
		gl_TessLevelOuter[0] = 100;
		gl_TessLevelOuter[1] = 100;
		gl_TessLevelOuter[2] = 100;
		gl_TessLevelOuter[3] = 100;
	}

	vec3 BN = cross(IN[gl_InvocationID].normal, IN[gl_InvocationID].tangent);

	OUT[gl_InvocationID].normal = normalize(IN[gl_InvocationID].normal);
	OUT[gl_InvocationID].tangent = normalize(IN[gl_InvocationID].tangent);
	OUT[gl_InvocationID].binormal = normalize(BN);
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}