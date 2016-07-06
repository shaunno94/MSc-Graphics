#version 420 core

uniform samplerCube cubeTex;
uniform float ambient;

in Vertex	
{
	vec3 normal;
} IN;

layout(location = 0) out vec4 diffuseColour;
layout(location = 2) out vec4 irradiance;
void main(void)	
{
	diffuseColour = texture(cubeTex, normalize(IN.normal));
	irradiance = vec4(ambient, 0.0, 0.0, 1.0);
}