/*
Author: Shaun Heald
Lake node fragment (reflection) shader. 
Calculates the reflection colour by sampling the skybox texture, based on a reflection ray.
*/

#version 420 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;

in Vertex	
{
	vec2 texCoord;
	vec2 texCoord_noScale;
	vec3 normal;
	vec3 worldPos;
	vec4 colour;
	vec3 tangent;
	vec3 binormal;
} IN;

layout(location = 0) out vec4 diffuseColour;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 irradiance;
layout(location = 3) out vec4 reflection;

void main(void)	
{
	//Tangent, Bi-Normal, Normal matrix
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	//Sample bump texture, multiply by TBN to convert from tangent space to world space and apply a scale and bias.
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord_noScale).rgb * 2.0 - 1.0));

	vec4 diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
	//Calculate ray from fragment to eye.
	vec3 incident = normalize(IN.worldPos - cameraPos);

	//Incident - 2.0 * dot(Normal, I) * Normal
	vec3 reflDir = reflect(incident, normalize(IN.normal));
	//Sample cube texture based on reflection vector.
	vec4 reflCol = texture(cubeTex, reflDir);
	
	//Write data to textures.
	reflection = vec4(reflCol.xyz, 1.0);
	diffuseColour = vec4(diffuse.xyz, 1.0);
	//Scale and bias normal to the range 0 - 1 becuase it is being stored in a texture.
	outNormal = vec4((normal.xyz * 0.5) + 0.5, 1.0);
	irradiance = vec4(0.25, 30.0, 0.0, 0.75);
}