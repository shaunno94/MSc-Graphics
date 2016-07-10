/*
Author: Shaun Heald
Tessellation control shader for heightmap.
Sets up the rendering pipeline, samples normal from texture, computes tangent and bi-normal.
Outputs data to TES.
*/

#version 420 core

uniform mat4 modelMatrix;
uniform sampler2D heights_nrm;
uniform int innerLevel;
uniform int outerLevel;

layout(vertices = 3) out;

in Vertex {
	vec2 texCoord;
} IN[];

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
} OUT[];

//Fast approximate method for calculating tangents.
vec3 ComputeApproxTan(in vec3 normal)
{
	vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
	vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

	if(length(c1) > length(c2))
	{
		return c1;
	}
	return c2;
}

void main(void)	
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = innerLevel;
		gl_TessLevelOuter[0] = outerLevel;
		gl_TessLevelOuter[1] = outerLevel;
		gl_TessLevelOuter[2] = outerLevel;
	}

	vec3 N = texture(heights_nrm, IN[gl_InvocationID].texCoord).xyz;
	vec3 T = ComputeApproxTan(N);
	vec3 BN = cross(T,N);

	OUT[gl_InvocationID].normal = normalize(N);
	OUT[gl_InvocationID].tangent = normalize(T);
	OUT[gl_InvocationID].binormal = normalize(BN);
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}