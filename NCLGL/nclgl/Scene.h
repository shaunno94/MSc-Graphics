#pragma once
#pragma once
#include "Camera.h"

class SceneNode;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void UpdateScene(float msec);
	virtual void DrawScene(bool shadowPass);
	virtual void LateDraw();

	unsigned int AddSceneObject(SceneNode* node);

	Matrix4& GetSceneViewMatrix() { return viewMatrix; }

protected:
	Camera* sceneCamera;
	std::vector<SceneNode*> sceneObjects;
	Matrix4 viewMatrix;
};