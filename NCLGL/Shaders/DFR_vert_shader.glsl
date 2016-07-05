#version 450 core

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
	if(dir == 0)
	{
		gl_Position	= MVP * vec4(position, 1.0);
		//invProjView = inverse(projMatrix * viewMatrix);
		//calculating inverse on CPU, rather than per fragment.
		invProjView = invPV;
	}
	else
	{
		gl_Position = projMatrix * vec4(position, 1.0);
		//invProjView = inverse(shadowViewProj);
		//inverse shadow view projection, calculated on the CPU.
		invProjView = shadowViewProj;
	}
}