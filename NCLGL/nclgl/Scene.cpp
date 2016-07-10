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

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}

	for (unsigned int i = 0; i < pointLights.size(); ++i)
	{
		delete pointLights[i];
	}

	for (unsigned int i = 0; i < sceneShaderProgs.size(); i++)
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

	//Store a pointer to the directional light in another vector for faster drawing of directional lights.
	if (dirLight)
		lightIndex.push_back(node);

	return sceneObjects.size() - 1;
}

unsigned int Scene::AddShaderProgram(Shader* shader)
{
	//Check if the shader program has compiled/linked.
	if (!shader->LinkProgram())
	{
		cout << "Initialisation failed...A shader program (" << shader->GetProgramName() << ") failed to compile." << endl;
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

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Update(msec);
	}
}

void Scene::DrawScene(bool shadowPass, bool lightPass)
{
	if (!lightPass)
	{
		for (unsigned int i = 0; i < sceneObjects.size(); i++)
		{
			if (shadowPass)
			{
				//Only draw objects which cast a shadow.
				if (sceneObjects[i]->IsShadowCaster())
					sceneObjects[i]->DrawNode(shadowPass);
			}
			//Draw everything except directional lights.
			else if (!sceneObjects[i]->IsDirectionalLight())
			{
				sceneObjects[i]->DrawNode(shadowPass);
			}
		}
	}
	else
	{
		//Only draws directional lights.
		for (unsigned int i = 0; i < lightIndex.size(); i++)
		{
			lightIndex[i]->DrawNode();
		}
	}
}

void Scene::LateDraw() {}