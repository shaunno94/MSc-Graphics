#pragma once

#include "OGLRenderer.h"

#define SHADER_VERTEX   0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2
#define SHADER_TCS 3
#define SHADER_TES 4

using namespace std;
class Shader	
{
public:
	Shader(string vertex, string fragment, string tcs = "", string tes = "", string geometry = "");
	~Shader(void);

	GLuint  GetProgram() { return program;}
	string	GetProgramName() { return progName; }
	bool	LinkProgram();

protected:
	bool	LoadShaderFile(string from, string &into);
	GLuint	GenerateShader(string from, GLenum type);
	void	SetDefaultAttributes();
	void	SetProgName(string vertex, string fragment, string tcs = "", string tes = "", string geometry = "");
	void	Append(string s);

	GLuint objects[5];
	GLuint program;
	string progName;
	bool loadFailed;
};

