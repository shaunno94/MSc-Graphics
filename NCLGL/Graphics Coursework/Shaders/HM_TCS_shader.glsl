#version 450 core

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

vec3 getTan(in vec3 normal)
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
	vec3 T = normalize(getTan(N));
	vec3 BN = normalize(cross(T,N));
	//inverse
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT[gl_InvocationID].normal = normalize(normalMatrix * N);
	OUT[gl_InvocationID].tangent = normalize(normalMatrix * T);
	OUT[gl_InvocationID].binormal = normalize(normalMatrix * BN);
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}