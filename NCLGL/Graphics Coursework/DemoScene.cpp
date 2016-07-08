#include "HeightMapNode.h"
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

	UI_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "vertex_shader.glsl", File_Locs::SHADER_DIR + "fragment_shader.glsl"));

	particle_shader_index = AddShaderProgram(new Shader(File_Locs::SHADER_DIR + "vertex_shader.glsl", File_Locs::SHADER_DIR + "emt_frag_shader.glsl",
		"", "", File_Locs::SHADER_DIR + "emt_geo_shader.glsl"));

	lightVol = new OBJMesh();

	if (!lightVol->LoadOBJMesh((File_Locs::MESH_DIR + ("ico.obj")).c_str()))
	{
		cout << "Initialisation failed...Light volume obj could not be loaded." << endl;
		system("pause");
		exit(1);
	}

	PoliceBox::CreateBoxInstance();
	skybox_index = AddSceneObject(new SkyBoxNode(sceneShaderProgs[skybox_shader_index]));
	heightmap_index = AddSceneObject(new HeightMapNode(sceneShaderProgs[heightmap_shader_index], heightMap_center));
	envLight_index = AddSceneObject(new EnvLight(sceneShaderProgs[envLight_shader_index], heightMap_center), true);
	AddSceneObject(new LakeNode(sceneShaderProgs[lake_shader_index], heightMap_center, sceneCamera, sceneObjects[skybox_index]->getTextureID(),
		static_cast<EnvLight*>(sceneObjects[envLight_index])->getEnvLight()));
	policeBox_index = AddSceneObject(new PoliceBox(sceneShaderProgs[policeBox_shader_index], pb_light, lightVol, heightMap_center, false));

	sceneCamera->SetPosition(Vector3(heightMap_center.x, 1500.0f, heightMap_center.z));

	sceneObjects[heightmap_index]->SetShadowCaster(true);
	sceneObjects[policeBox_index]->SetShadowCaster(true);
	sceneObjects[envLight_index]->SetDirectionalLight(true);

	shadowPersp = Matrix4::Perspective(100.0f, 15000.0f, 1.0f, 45.0f);

	InitialiseLights();

	HUD = new UI(SceneNode::context->GetRenderWidth(), SceneNode::context->GetRenderHeight(), true, 
		SceneNode::context->GetPixelPitch(), sceneShaderProgs[UI_shader_index]);

	HUD->AddText("Memory Used: " + to_string((Mesh::GetBytesUsed() + OGLRenderer::bytes_used) / (1024 * 1024)) + "(MB)");
	HUD->AddText("Camera Controls: W, A, S, D, Q, E");
	HUD->AddText("Tardis Controls: I, J, K, L, U, O");
	HUD->AddText("Light Controls: Rotate (1, 2), View (P)");
	HUD->AddText("Toggle Wireframe: M");
	HUD->AddText("Toggle Blur: N");

	emitter = new ParticleEmitter(sceneShaderProgs[particle_shader_index]);
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

	delete emitter;
	emitter = nullptr;

	delete lightVol;
	lightVol = nullptr;
}

void DemoScene::UpdateScene(float msec)
{
	emitter->Update(msec);
	HUD->Update(msec);
	Scene::UpdateScene(msec);
}

void DemoScene::DrawScene(bool shadowPass, bool lightPass)
{	
	if (shadowPass)
	{
		SceneNode::context->UpdateProjMatrix(shadowPersp);

		lightVM = Matrix4::Translation(Vector3(0, 0, -10000.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 0, 1)) *
			Matrix4::Rotation(sceneLightRot, Vector3(0.0f, 1.0f, 0.0f)) *
			Matrix4::BuildViewMatrix(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)) *
			Matrix4::Translation(Vector3(-heightMap_center.x, 0.0f, -heightMap_center.z));

		SceneNode::context->UpdateViewMatrix(lightVM);

		static_cast<EnvLight*>(sceneObjects[envLight_index])->setPos(Matrix4::Inverse(lightVM).GetPositionVector());
		static_cast<EnvLight*>(sceneObjects[envLight_index])->setShadowMatrix(biasMatrix * 
			(SceneNode::context->GetProjMat() * lightVM * static_cast<HeightMapNode*>(sceneObjects[heightmap_index])->getHM_ModelMatrix()));
	}
	
	Scene::DrawScene(shadowPass, lightPass);

	if (shadowPass)
	{
		SceneNode::context->SwitchToPerspective();
		static_cast<EnvLight*>(sceneObjects[envLight_index])->setShadowProjView(SceneNode::context->GetProjMat() * viewMatrix);
		
		if (!viewLight)
			SceneNode::context->UpdateViewMatrix(viewMatrix);
	}	
	else if(!lightPass && !blur)
	{
		emitter->Draw();
	}
}

void DemoScene::LateDraw()
{
	HUD->Draw();
	Scene::LateDraw();
}