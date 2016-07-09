/*
Author: Shaun Heald
Deferred Vertex Shader
*/

#version 420 core

uniform mat4 MVP;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 shadowViewProj;
uniform mat4 invPV;
uniform int dir;

in vec3 position;

out mat4 invProjView;

void main(void)	
{
	//Point light - does not cast shadows
	if(dir == 0)
	{
		gl_Position	= MVP * vec4(position, 1.0);
		//calculating inverse on CPU, rather than per fragment.
		invProjView = invPV;
	}
	//Directional light - casts shadows
	else
	{
		gl_Position = projMatrix * vec4(position, 1.0);
		//inverse shadow view projection, calculated on the CPU.
		invProjView = shadowViewProj;
	}
}