#version 450 core

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

	vec3 BN = normalize(cross(IN[gl_InvocationID].normal, IN[gl_InvocationID].tangent));
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT[gl_InvocationID].normal = normalize(normalMatrix * IN[gl_InvocationID].normal);
	OUT[gl_InvocationID].tangent = normalize(normalMatrix * IN[gl_InvocationID].tangent);
	OUT[gl_InvocationID].binormal = normalize(normalMatrix * BN);
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}