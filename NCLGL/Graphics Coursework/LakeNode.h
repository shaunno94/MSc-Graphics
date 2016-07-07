#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/Camera.h"

class LakeNode : public SceneNode
{
public:
	LakeNode(Shader* shader, Vector3 offset, Camera* cam, GLuint SB_texID, Light* l);
	virtual ~LakeNode();

	virtual void Update(float msec);
	virtual void DrawNode(bool shadowPass = false);
		
protected:
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
