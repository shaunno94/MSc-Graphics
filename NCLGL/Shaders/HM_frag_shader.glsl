/*
Author: Shaun Heald
Heightmap fragment shader, samples multiple textures and blends between them to calculate albedo colour.
Calculates normal by multiplying TBN matrix with a sampled vector from a normal map.
*/

#version 420 core

uniform sampler2D rockTex;
uniform sampler2D snowTex;
uniform sampler2D grassTex;
uniform sampler2D bumpTex;

in Vertex	
{
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} IN;

layout(location = 0) out vec4 diffuseColour;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 irradiance;

void main(void)	
{
	//Tangent, normal, bi-normal matrix.
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	//Sample bump texture, multiply by TBN to convert from tangent space to world space and apply a scale and bias.
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

	//Diffuse Colour
	float val = clamp(normalize(IN.worldPos).y, 0.0, 1.0);
	//Blend between textures based on height of vertex.
	vec4 blend1 = mix(texture(grassTex, IN.texCoord), texture(rockTex, IN.texCoord), smoothstep(0.0, 0.15, val));
	vec4 blend2 = mix(texture(rockTex, IN.texCoord), texture(snowTex, IN.texCoord), smoothstep(0.1, 0.25, val));
	vec4 diffuse = mix(blend1, blend2, smoothstep(0.0, 0.25, val));

	//Output data to textures.
	diffuseColour = diffuse;
	//Scale and bias normal to the range 0 - 1 becuase it is being stored in a texture.
	outNormal = vec4((normal.xyz * 0.5) + 0.5, 1.0);
	irradiance = vec4(0.3, 18.0, 0.0, 1.0);
}