#include "PoliceBox.h"
#include "HeightMapNode.h"
#include "SkyBoxNode.h"
#include "LakeNode.h"
#include "EnvLight.h"
#include "ParticleEmitter.h"
#include "UI.h"
#include "DemoScene.h"

DemoScene::DemoScene()
{
	PoliceBox::CreateBoxInstance();
	AddSceneObject(new HeightMapNode(sceneCamera, heightMap_center));
	AddSceneObject(new PoliceBox(pb_light, heightMap_center, false));
	skybox_index = AddSceneObject(new SkyBoxNode());
	envLight_index = AddSceneObject(new EnvLight(shaderProgs[LIGHT_SHADER], heightMap_center));
	AddSceneObject(new LakeNode(heightMap_center, sceneCamera, sceneObjects[skybox_index]->getTextureID(), static_cast<EnvLight*>(sceneObjects[envLight_index])->getEnvLight()));

	sceneCamera->SetPosition(Vector3(heightMap_center.x, 1500.0f, heightMap_center.z));
}

DemoScene::~DemoScene()
{

}

void DemoScene::UpdateScene(float msec)
{
	Scene::UpdateScene(msec);
}

void DemoScene::DrawScene(bool shadowPass)
{
	Scene::DrawScene(shadowPass);
}

void DemoScene::LateDraw()
{
	Scene::LateDraw();
}