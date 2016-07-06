#version 420 core

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 revProjMat;
//uniform int reverse;

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;

out Vertex 
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
} OUT;

void main(void)	
{
	vec4 pos = vec4(1.0);
	//if(reverse == 0)
	//{
		pos	= MVP * vec4(position, 1.0);
	//}
	//else
	//{
		//pos = (revProjMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	//}	
	gl_Position = pos;
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normal = normal;
}