#version 420 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

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
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord_noScale).rgb * 2.0 - 1.0));

	vec4 diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
	vec3 incident = normalize(IN.worldPos - cameraPos);
	
	float fresnel =  1.0 - dot(-incident, IN.normal);
	fresnel = smoothstep(0.0, 0.6, fresnel);

	vec3 reflDir = reflect(incident, normalize(IN.normal));
	//vec3 reftDir = refract(incident, normalize(IN.normal), 0.9);
	vec4 reflCol = texture(cubeTex, reflDir);
	//vec4 reftCol = texture(cubeTex, reftDir) * 0.5;

	
	//reflection = vec4((reflCol.xyz * fresnel), 1.0);
	//diffuseColour = vec4((diffuse.xyz * (1.0 - fresnel)), 1.0);
	reflection = vec4(reflCol.xyz, 1.0);
	diffuseColour = vec4(diffuse.xyz, 1.0);
	outNormal = vec4((normal.xyz * 0.5) + 0.5, 1.0);
	irradiance = vec4(0.25, 30.0, 0.0, 0.75);
}