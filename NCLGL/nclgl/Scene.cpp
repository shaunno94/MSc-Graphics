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
	lightIndex.clear();
}

unsigned int Scene::AddSceneObject(SceneNode* node, bool dirLight)
{
	sceneObjects.push_back(node);

	if (dirLight)
		lightIndex.push_back(sceneObjects.size() - 1);

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
	viewMatrix = sceneCamera->BuildViewMatrix();
	SceneNode::context->UpdateViewMatrix(viewMatrix);

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Update(msec);
	}
}

void Scene::DrawScene(bool shadowPass, bool lightPass)
{
	if (!lightPass)
	{
		for (int i = 0; i < sceneObjects.size(); i++)
		{
			if (shadowPass)
			{
				if (sceneObjects[i]->IsShadowCaster())
					sceneObjects[i]->DrawNode(shadowPass);
			}
			else if (!sceneObjects[i]->IsDirectionalLight())
			{
				sceneObjects[i]->DrawNode(shadowPass);
			}
		}
	}
	else
	{
		for (int i = 0; i < lightIndex.size(); i++)
		{
			sceneObjects[lightIndex[i]]->DrawNode();
		}
	}
}

void Scene::LateDraw()
{

}