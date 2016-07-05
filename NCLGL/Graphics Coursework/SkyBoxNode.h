#pragma once
#include "../nclgl/SceneNode.h"

class SkyBoxNode : public SceneNode
{
public:
	SkyBoxNode();
	virtual ~SkyBoxNode();

	virtual void DrawNode(bool shadowPass = false);
	virtual void Update(float msec);

	inline void incAmbient() {
		if(ambient < 1.0f)
			ambient += (2.0f / 360.0f);
	}

	inline void decAmbient() { 
		if(ambient > 0.25f) 
			ambient -= (2.0f / 360.0f); }

protected:
	const char* SKYBOX_BACK = "..\\Textures\\ThickCloudsWaterBack.png";
	const char* SKYBOX_FRONT = "..\\Textures\\ThickCloudsWaterFront.png";
	const char* SKYBOX_LEFT = "..\\Textures\\ThickCloudsWaterLeft.png";
	const char* SKYBOX_RIGHT = "..\\Textures\\ThickCloudsWaterRight.png";
	const char* SKYBOX_TOP = "..\\Textures\\ThickCloudsWaterUp.png";
	const char* SKYBOX_BOTTOM = "..\\Textures\\ThickCloudsWaterDown.png";

	//Skybox shaders
	const char* SKYBOX_FRAG_SHADER = "..\\Shaders\\SB_frag_shader.glsl";
	const char* SKYBOX_VERT_SHADER = "..\\Shaders\\SB_vertex_shader.glsl";

	float ambient = 1.0f;

	GLuint cubeTex_loc;
	GLuint ambient_loc;
};