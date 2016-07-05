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

	inline Vector3& getLightPos() { return dirLight->getPos(); }
	inline Light*& getEnvLight() { return dirLight; }
	inline void setPos(Vector3 v) { dirLight->setPos(v); }

	inline void setShadowProjView(Matrix4 m) { shadowVP = m; }
	inline void setShadowMatrix(Matrix4 m) { shadowProj = m; }

protected:
	Matrix4 shadowProj;
	Matrix4 shadowVP;

	Light* dirLight;
	float lightRot;
	Vector3 center;

	GLuint dir_loc;
	GLuint shadowVP_loc;
	GLuint shadowMatrix_loc;

	Vector4 lightCol = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
};