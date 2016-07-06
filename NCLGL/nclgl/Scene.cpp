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

	for (int i = 0; i < pointLights.size(); ++i)
	{
		delete pointLights[i];
	}

	for (int i = 0; i < sceneShaderProgs.size(); i++)
	{
		delete sceneShaderProgs[i];
	}

	sceneShaderProgs.clear();
	pointLights.clear();
	sceneObjects.clear();
}

unsigned int Scene::AddSceneObject(SceneNode* node)
{
	sceneObjects.push_back(node);
	return sceneObjects.size() - 1;
}

unsigned int Scene::AddShaderProgram(Shader* shader)
{
	if (!shader->LinkProgram())
	{
		cout << "Initialisation failed...A shader program failed to compile." << endl;
		system("pause");
		exit(1);
	}

	sceneShaderProgs.push_back(shader);
	return sceneShaderProgs.size() - 1;
}

void Scene::UpdateScene(float msec)
{
	sceneCamera->UpdateCamera(msec);
	SceneNode::context->UpdateViewMatrix(sceneCamera->BuildViewMatrix());

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