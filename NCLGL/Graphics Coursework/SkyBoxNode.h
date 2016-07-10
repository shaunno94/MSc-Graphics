/*
Author: Shaun Heald
This class extends Scene Node and represents the skybox object in the demo scene.
*/

#pragma once
#include "../nclgl/SceneNode.h"

class SkyBoxNode : public SceneNode
{
public:
	SkyBoxNode(Shader* shader);
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
	float ambient = 1.0f;

	GLuint cubeTex_loc;
	GLuint ambient_loc;
};