#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/Camera.h"

class LakeNode : public SceneNode
{
public:
	LakeNode(Vector3 offset, Camera* cam, GLuint SB_texID, Light*& l);
	virtual ~LakeNode();

	virtual void Update(float msec);
	virtual void DrawNode(bool shadowPass = false);
		
protected:
	//Textures
	const char* WATER_TEX = "..\\Textures\\water.jpg";
	const char* WATER_BUMP = "..\\Textures\\water_NRM.jpg";
	const char* WATER_HEIGHT = "..\\Textures\\water_height.png";
	const char* WATER_HEIGHT2 = "..\\Textures\\water_height2.png";

	//Reflection shaders
	const char* REFLECT_FRAG_SHADER = "..\\Shaders\\reflect_frag_shader.glsl";
	const char* REFLECT_VERT_SHADER = "..\\Shaders\\reflect_vert_shader.glsl";
	const char* REFLECT_TCS_SHADER = "..\\Shaders\\reflect_TCS_shader.glsl";
	const char* REFLECT_TES_SHADER = "..\\Shaders\\reflect_TES_shader.glsl";

	GLuint cameraPos_loc;
	GLuint diffuseTex_loc;
	GLuint cubeTex_loc;
	GLuint bumpTex_loc;
	GLuint water_height_loc;
	GLuint water_height2_loc;
	GLuint heightMatrix_loc;
	GLuint heightMatrix2_loc;
	GLuint texMat_noScale_loc;

	GLuint water_height;
	GLuint water_height2;

	Matrix4 heightMatrix;
	Matrix4 heightMatrix2;

	GLuint skybox_tex;

	float waterRot = 0.0f;
	Camera* camera;
	Light* envLight;
};
