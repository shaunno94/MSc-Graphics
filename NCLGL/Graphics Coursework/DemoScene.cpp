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
	skybox_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "SB_vertex_shader.glsl", File_Locs::SHADER_DIR + "SB_frag_shader.glsl"));
	
	policeBox_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "PB_vert_shader.glsl", File_Locs::SHADER_DIR + "PB_frag_shader.glsl"));
	
	envLight_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "DFR_vert_shader.glsl", File_Locs::SHADER_DIR + "DFR_frag_shader.glsl"));
	
	lake_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "reflect_vert_shader.glsl", File_Locs::SHADER_DIR + "reflect_frag_shader.glsl",
		File_Locs::SHADER_DIR + "reflect_TCS_shader.glsl", File_Locs::SHADER_DIR + "reflect_TES_shader.glsl"));
	
	heightmap_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "HM_vert_shader.glsl", File_Locs::SHADER_DIR + "HM_frag_shader.glsl",
		File_Locs::SHADER_DIR + "HM_TCS_shader.glsl", File_Locs::SHADER_DIR + "HM_TES_shader.glsl"));

	PoliceBox::CreateBoxInstance();
	heightmap_index = AddSceneObject(new HeightMapNode(sceneShaderProgs[heightmap_shader_index], sceneCamera, heightMap_center));
	policeBox_index = AddSceneObject(new PoliceBox(sceneShaderProgs[policeBox_shader_index], pb_light, heightMap_center, false));
	skybox_index = AddSceneObject(new SkyBoxNode(sceneShaderProgs[skybox_shader_index]));
	envLight_index = AddSceneObject(new EnvLight(sceneShaderProgs[envLight_shader_index], heightMap_center));
	AddSceneObject(new LakeNode(sceneShaderProgs[lake_shader_index], heightMap_center, sceneCamera, sceneObjects[skybox_index]->getTextureID(), 
		static_cast<EnvLight*>(sceneObjects[envLight_index])->getEnvLight()));

	sceneCamera->SetPosition(Vector3(heightMap_center.x, 1500.0f, heightMap_center.z));

	lightVol = new OBJMesh();

	if (!lightVol->LoadOBJMesh((File_Locs::MESH_DIR + ("ico.obj")).c_str()))
	{
		cout << "Initialisation failed...Light volume obj could not be loaded." << endl;
		system("pause");
		exit(1);
	}

	sceneObjects[heightmap_index]->IsShadowCaster(true);
	sceneObjects[policeBox_index]->IsShadowCaster(true);
	sceneObjects[envLight_index]->IsDirectionalLight(true);

	InitialiseLights();
}

void DemoScene::InitialiseLights()
{
	for (unsigned int x = 0; x < LIGHTNUM; ++x)
	{
		for (unsigned int z = 0; z < LIGHTNUM; ++z)
		{
			float xPos = (heightMap_center.x / float(LIGHTNUM - 1)) * x;
			float zPos = (heightMap_center.z / float(LIGHTNUM - 1)) * z;

			float r = 0.5f + (float)(rand() % 129) / 128.0f;
			float g = 0.5f + (float)(rand() % 129) / 128.0f;
			float b = 0.5f + (float)(rand() % 129) / 128.0f;

			float radius = heightMap_center.x / float(LIGHTNUM);
			pointLights.push_back(new Light(Vector3(xPos, 850.0f, zPos),
				Vector3(heightMap_center.x, 0.0f, heightMap_center.z), Vector4(r, g, b, 1.0f), radius, false, 0.5, lightVol));
		}
	}
	pointLights.push_back(pb_light);
}

DemoScene::~DemoScene()
{
	PoliceBox::DeleteBoxInstance();

	delete lightVol;
	lightVol = nullptr;
}

void DemoScene::UpdateScene(float msec)
{
	Scene::UpdateScene(msec);
}

void DemoScene::DrawScene(bool shadowPass)
{	
	if (shadowPass)
	{
		SceneNode::context->UpdateViewMatrix(Matrix4::Translation(Vector3(0, 0, -10000)) * Matrix4::Rotation(90.0f, Vector3(0, 0, 1)) *
			Matrix4::Rotation(sceneLightRot, Vector3(0, 1, 0)) *
			Matrix4::BuildViewMatrix(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(0, 0, 1)) *
			Matrix4::Translation(Vector3(-heightMap_center.x, 0, -heightMap_center.z)));
		
		static_cast<EnvLight*>(sceneObjects[envLight_index])->setPos(Matrix4::Inverse(viewMatrix).GetPositionVector());
		static_cast<EnvLight*>(sceneObjects[envLight_index])->setShadowMatrix(biasMatrix * (SceneNode::context->GetProjMat() * viewMatrix * static_cast<HeightMapNode*>(sceneObjects[heightmap_index])->getHM_ModelMatrix()));
	}

	Scene::DrawScene(shadowPass);
}

void DemoScene::LateDraw()
{
	Scene::LateDraw();
}