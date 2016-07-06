#pragma once
/*
Class:OGLRenderer
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Abstract base class for the graphics tutorials. Creates an OpenGL 
3.2 CORE PROFILE rendering context. Each lesson will create a renderer that 
inherits from this class - so all context creation is handled automatically,
but students still get to see HOW such a context is created.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include "Common.h"

#include <string>
#include <fstream>
#include <vector>

#include "GL/glew.h"
#include "GL/wglew.h"

#include "SOIL.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Window.h"
#include "Light.h"

#include "Shader.h"		
#include "Mesh.h"

using std::vector;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")

static const float biasValues[16] = 
{
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
};
static const Matrix4 biasMatrix(const_cast<float*>(biasValues));

class Shader;

class OGLRenderer	
{

public:
	friend class Window;
	OGLRenderer(Window &parent);
	virtual ~OGLRenderer(void);

	virtual void	RenderScene(float msec)		= 0;
	void			SwapBuffers();

	bool			HasInitialised() const;	
	void			SetCurrentShader(Shader*s);
	void			SetShaderLight(Light* l);
	void			UpdateModelMatrix(Matrix4& m) { modelMatrix = m; }
	void			UpdateViewMatrix(Matrix4& m)	{ viewMatrix = m; }
	void			UpdateProjMatrix(Matrix4& m)	{ projMatrix = m; }
	void			UpdateTextureMatrix(Matrix4& m) { textureMatrix = m; }
	Matrix4&		GetProjMat(){ return projMatrix; }
	void			UpdateShaderMatrices();
	void			SwitchToOrtho();
	void			SwitchToPerspective();
	
	static void		SetTextureRepeating(GLuint target, bool state);
	static void		SetTextureFiltering(GLuint target, bool state, bool AF);
	static void		GenerateMipMaps(GLuint target);

protected:
	virtual void	Resize(int x, int y);	
	virtual void	UpdateScene(float msec);

	void			SetCubeMapParams(GLuint target);

	Shader* currentShader;
	
	Matrix4 projMatrix;		//Projection matrix
	Matrix4 modelMatrix;	//Model matrix. NOT MODELVIEW
	Matrix4 viewMatrix;		//View matrix
	Matrix4 MVP;			//Combined model, view, projection matrix
	Matrix4 VP;				//View Projection matrix
	Matrix4 textureMatrix;	//Texture matrix

	int		width;			//Render area width (not quite the same as window width)
	int		height;			//Render area height (not quite the same as window height)
	bool	init;			//Did the renderer initialise properly?

	HDC		deviceContext;	//...Device context?
	HGLRC	renderContext;	//Permanent Rendering Context

	static Shader*		  debugDrawShader;

	GLfloat AF_largest;
	const float FOV = 45.0f;
	const float NEAR_PLANE = 1.0f;
	const float FAR_PLANE = 15000.0f;
};