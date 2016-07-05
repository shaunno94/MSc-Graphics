#include "Scene.h"
#include "SceneNode.h"

Scene::Scene()
{
	sceneCamera = new Camera();
}

Scene::~Scene()
{
	if (sceneCamera)
	{
		delete sceneCamera;
		sceneCamera = nullptr;
	}

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}

	sceneObjects.clear();
}

unsigned int Scene::AddSceneObject(SceneNode* node)
{
	sceneObjects.push_back(node);
	return sceneObjects.size() - 1;
}

void Scene::UpdateScene(float msec)
{
	sceneCamera->UpdateCamera(msec);
	viewMatrix = sceneCamera->BuildViewMatrix();

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Update(msec);
	}
}

void Scene::DrawScene(bool shadowPass)
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->DrawNode(shadowPass);
	}
}

void Scene::LateDraw()
{

}