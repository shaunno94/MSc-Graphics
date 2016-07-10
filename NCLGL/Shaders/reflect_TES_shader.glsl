/*
Author: Shaun Heald
Lake node tessellation evaluation shader. 
Takes the newly generated primitive (quads) and appropriately calulcates the
per vertex data ready for the fragment shader.
*/

#version 420 core

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform sampler2D waterHeights;
uniform sampler2D waterHeights_2;
uniform mat4 textureMatrix;
uniform mat4 heightMatrix;
uniform mat4 heightMatrix_2;

layout(quads, equal_spacing, ccw) in;

in Vertex {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec4 colour;
} IN[];

out Vertex {
	vec2 texCoord;
	vec2 texCoord_noScale;
	vec3 normal;
	vec3 worldPos;
	vec4 colour;
	vec3 tangent;
	vec3 binormal;
} OUT;

//Linear interpolation - vec3 (for quads)
vec3 lerp3(in vec3 v0, in vec3 v1, in vec3 v2, in vec3 v3)
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	vec3 a = mix(v0, v1, u);
	vec3 b = mix(v2, v3, u);
	return mix(a, b, v);
}

//Linear interpolation - vec2 (for quads)
vec2 lerp2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	vec2 a = mix(v0, v2, u);
	vec2 b = mix(v1, v3, u);
	return mix(a, b, v);
}

const float SCALE = 1700.0;

void main(void)	
{
	vec3 combinedPos = lerp3(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, 
				gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);

	vec2 combinedTex = lerp2(IN[0].texCoord, IN[1].texCoord, IN[2].texCoord, IN[3].texCoord);

	vec3 combinedNormal = normalize(lerp3(IN[0].normal, IN[1].normal, IN[2].normal, IN[3].normal));

	vec3 combinedTangent = normalize(lerp3(IN[0].tangent, IN[1].tangent, IN[2].tangent, IN[3].tangent));

	vec3 combinedBN = normalize(lerp3(IN[0].binormal, IN[1].binormal, IN[2].binormal, IN[3].binormal));
	
	//Multiply the new texture co-ordinates by each texture height matrix and update the vertex Y, to make the water look more dynamic.
	vec2 tmpTex = (heightMatrix * vec4(combinedTex, 0.0, 1.0)).xy;
	vec2 tmpTex_2 = (heightMatrix_2 * vec4(combinedTex, 0.0, 1.0)).xy;
	float val = mix(texture(waterHeights, tmpTex).r, texture(waterHeights_2, tmpTex_2).r, 0.5);
	combinedPos.y += val * SCALE;
	
	OUT.texCoord = (textureMatrix * vec4(combinedTex, 0.0, 1.0)).xy;
	OUT.texCoord_noScale = (heightMatrix * vec4(combinedTex, 0.0, 1.0)).xy;
	OUT.normal = combinedNormal;
	OUT.tangent = combinedTangent;
	OUT.binormal = combinedBN;
	OUT.worldPos = (modelMatrix * vec4(combinedPos, 1.0)).xyz;
	OUT.colour = IN[0].colour;

	gl_Position = MVP * vec4(combinedPos, 1.0);
}