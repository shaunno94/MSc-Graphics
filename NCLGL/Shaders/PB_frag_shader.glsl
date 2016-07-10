/*
Author: Shaun Heald
Simple fragment shader, can sample from texture or use colour to compute fragment colour.
Outputs normal to texture to be used for lighting calculations.
*/

#version 420 core

uniform sampler2D tardisTex;
uniform int usingTex;
uniform vec4 nodeColour;
uniform float opacity;

in Vertex	
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
} IN;

layout(location = 0) out vec4 diffuseColour;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 irradiance;

void main(void)
{
	if (usingTex == 0)
	{
		diffuseColour = nodeColour;
	}
	else
	{
		diffuseColour = texture(tardisTex, IN.texCoord);
	}

	//Alpha
	diffuseColour.a = opacity;
	//Scale and bias normal to the range 0 - 1 becuase it is being stored in a texture.
	outNormal = vec4((IN.normal.xyz * 0.5) + 0.5, 1.0);
	irradiance = vec4(0.8, 60.0, 0.0, opacity);
}