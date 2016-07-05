#version 450 core

uniform sampler2D tardisTex;
uniform int usingTex;
uniform vec4 nodeColour;
uniform float opacity;
//uniform int reverse;

in Vertex	
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
} IN;

layout(location = 0) out vec4 diffuseColour;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 irradiance;
//layout(location = 4) out vec4 reverseColour;
void main(void)	
{
//	if(reverse == 0)
	//{
		if(usingTex == 0)
		{
			diffuseColour = nodeColour;
		}
		else
		{
			diffuseColour = texture(tardisTex, IN.texCoord);
		}
		diffuseColour.a = opacity;
		outNormal = vec4((IN.normal.xyz * 0.5) + 0.5, 1.0);
		irradiance = vec4(0.8, 60.0, 0.0, opacity);
	//}
/*	else
	{
		if(usingTex == 0)
		{
			reverseColour = nodeColour;
		}
		else
		{
			reverseColour = texture(tardisTex, IN.texCoord);
		}
		reverseColour.a = opacity;
	}*/
}