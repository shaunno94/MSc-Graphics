/*
Author: Shaun Heald
This class loads shader programs from a plain text files, compiles, links and creates a OpenGL shader program.
*/

#pragma once
#include "OGLRenderer.h"

enum SHADER_STAGES
{
	SHADER_VERTEX, SHADER_FRAGMENT, SHADER_GEOMETRY, 
		SHADER_TCS, SHADER_TES, SHADER_STAGES_MAX
};

using namespace std;

class Shader	
{
public:
	Shader(string vertex, string fragment, string tcs = "", string tes = "", string geometry = "");
	~Shader(void);

	//Returns this programs unique ID.
	GLuint GetProgram()	const		{ return program;}
	//Returns this programs name which consists of the individual shader program names.
	string GetProgramName() const	{ return progName; }
	//Returns an index to the resource location.
	GLuint GetUniformLocation(char* name) const;
	//Has this program linked successfully?
	bool LinkProgram();

protected:
	//Loads shader source from file.
	bool LoadShaderFile(string from, string &into);
	//Compiles shader source code.
	GLuint GenerateShader(string from, GLenum type);
	//Setup default attribute locations for this shader program.
	void SetDefaultAttributes();
	/* Utility functions which create the programs name. */
	void SetProgName(string vertex, string fragment, string tcs = "", string tes = "", string geometry = "");
	void Append(string s);

	//Handles to each shader stage.
	GLuint objects[SHADER_STAGES_MAX];
	//Program ID.
	GLuint program;
	string progName;
	bool loadFailed;
};