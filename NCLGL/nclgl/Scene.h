/*
Author: Shaun Heald
This class represents a generic scene, it holds a vector of scene nodes,
a vector of shaders which are used by the scene nodes, and a vector of point lights.
The scene class provides update, draw and late draw (after post-processing e.g. UI) methods which can be overriden.
*/

#pragma once
#include "Camera.h"

class SceneNode;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	//Updates the camera & view matrix, and updates any scene nodes in the scene objects vector.
	virtual void UpdateScene(float msec);
	//Draws any scene nodes in the scene objects vector.
	virtual void DrawScene(bool shadowPass, bool lightPass = false);
	virtual void LateDraw();

	//Adds a Scene Node object to the scene object vector.
	unsigned int AddSceneObject(SceneNode* node, bool dirLight = false);
	//Adds a Shader to the scene shader vector.
	unsigned int AddShaderProgram(Shader* shader);
	
	//Returns this scenes camera.
	inline Camera* GetSceneCamera() { return sceneCamera; }
	//Point light utility functions.
	inline unsigned int GetNumberOfPointLights() { return pointLights.size(); }
	inline Light* GetPointLight(unsigned int index) { return index < pointLights.size() ? pointLights[index] : nullptr; }

protected:
	Camera* sceneCamera;
	std::vector<SceneNode*> sceneObjects;
	std::vector<SceneNode*> lightIndex;
	std::vector<Shader*> sceneShaderProgs;
	std::vector<Light*> pointLights;
	Matrix4 viewMatrix;
};