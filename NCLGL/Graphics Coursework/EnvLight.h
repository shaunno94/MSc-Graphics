/*
Author: Shaun Heald
This class extends Scene Node and represents a directional light.
The directional light casts shadows, therefore some extra matrices are stored and used by this class.
*/

#pragma once
#include "../nclgl//Light.h"
#include "../nclgl/SceneNode.h"

class EnvLight : public SceneNode
{
public:
	EnvLight(Shader* shader, Vector3 pos);
	~EnvLight();

	virtual void DrawNode(bool shadowPass = false);
	virtual void Update(float msec);

	inline Vector3& getLightPos()	{ return dirLight->getPos(); }
	inline Light* getEnvLight()		{ return dirLight; }
	
	//Utility functions to update the nodes state.
	inline void setPos(Vector3 v)	{ dirLight->setPos(v); }
	inline void setShadowProjView(Matrix4 m)	{ shadowVP = m; }
	inline void setShadowMatrix(Matrix4 m)		{ shadowProj = m; }

protected:
	Matrix4 shadowProj;
	Matrix4 shadowVP;

	Light* dirLight;
	float lightRot;
	Vector3 center;

	GLuint dir_loc;
	GLuint shadowVP_loc;
	GLuint shadowMatrix_loc;

	Vector4 lightColour = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
};