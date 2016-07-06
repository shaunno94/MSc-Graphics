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
	unsigned int AddShaderProgram(Shader* shader);
	Camera* GetSceneCamera() { return sceneCamera; }
	unsigned int GetNumberOfPointLights() { return pointLights.size(); }
	Light* GetPointLight(int index) { return index < pointLights.size() ? pointLights[index] : nullptr; }

	Matrix4& GetSceneViewMatrix() { return viewMatrix; }

protected:
	Camera* sceneCamera;
	std::vector<SceneNode*> sceneObjects;
	std::vector<Shader*> sceneShaderProgs;
	std::vector<Light*> pointLights;
	Matrix4 viewMatrix;
};