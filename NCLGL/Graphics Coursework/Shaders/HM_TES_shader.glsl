#version 450 core

uniform mat4 MVP;
uniform mat4 modelMatrix;

layout(triangles, equal_spacing, ccw) in;

in Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
} IN[];

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} OUT;

vec3 lerp3(in vec3 v1, in vec3 v2, in vec3 v3)
{
	vec3 p0 = gl_TessCoord.x * v1;
	vec3 p1 = gl_TessCoord.y * v2;
	vec3 p2 = gl_TessCoord.z * v3;
	return p0 + p1 + p2;
}

vec2 lerp2(in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 p0 = gl_TessCoord.x * v1;
	vec2 p1 = gl_TessCoord.y * v2;
	vec2 p2 = gl_TessCoord.z * v3;
	return p0 + p1 + p2;
}

void main(void)	
{
	vec3 combinedPos = lerp3(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, 
				gl_in[2].gl_Position.xyz);

	vec2 combinedTex = lerp2(IN[0].texCoord, IN[1].texCoord, IN[2].texCoord);

	vec3 combinedNormal = normalize(lerp3(IN[0].normal, IN[1].normal, IN[2].normal));

	vec3 combinedTangent = normalize(lerp3(IN[0].tangent, IN[1].tangent, IN[2].tangent));

	vec3 combinedBN = normalize(lerp3(IN[0].binormal, IN[1].binormal, IN[2].binormal));
	
	OUT.texCoord = combinedTex;
	OUT.normal = combinedNormal;
	OUT.tangent = combinedTangent;
	OUT.binormal = combinedBN;
	OUT.worldPos = (modelMatrix * vec4(combinedPos, 1.0)).xyz;
	gl_Position	= MVP * vec4(combinedPos, 1.0);
}