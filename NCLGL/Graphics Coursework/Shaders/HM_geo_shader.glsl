#version 450 core

uniform mat4 MVP;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex {
	vec2 texCoord;
	vec3 worldPos;
	mat3 normalMatrix;
} IN[];

out Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} OUT;

void main(void)	
{
	vec2 coord1 = IN[1].texCoord - IN[0].texCoord;
	vec2 coord2 = IN[2].texCoord - IN[0].texCoord;
	vec3 v1 = IN[1].worldPos.xyz - IN[0].worldPos.xyz;
	vec3 v2 = IN[2].worldPos.xyz - IN[0].worldPos.xyz;
	vec3 N = normalize(cross(v1, v2));

	vec3 axis = vec3(v1 * coord2.y - v2 * coord1.y);
	float fac = 1.0 / (coord1.x * coord2.y - coord2.x * coord1.y);
	vec3 T = normalize(axis * fac);
	vec3 BN = normalize(cross(N, T));

	vec3 tmpNorm;
	vec3 tmpTan;

	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		OUT.texCoord = IN[i].texCoord;
		OUT.worldPos = IN[i].worldPos;

		tmpNorm = IN[i].normalMatrix * N;
		tmpTan = IN[i].normalMatrix * T;
		OUT.normal = N;
		OUT.tangent = T;
		OUT.binormal = normalize(IN[i].normalMatrix * normalize(cross(tmpNorm, tmpTan)));
		EmitVertex();
	}
}