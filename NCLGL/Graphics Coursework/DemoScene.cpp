#include "HeightMapNode.h"
#include "LakeNode.h"
#include "EnvLight.h"
#include "ParticleEmitter.h"
#include "UI.h"
#include "DemoScene.h"

DemoScene::DemoScene()
{
	//Setup various shader programs.
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

	//Setup various scene elements.
	PoliceBox::CreateBoxInstance();
	skybox_index = AddSceneObject(new SkyBoxNode(sceneShaderProgs[skybox_shader_index]));
	heightmap_index = AddSceneObject(new HeightMapNode(sceneShaderProgs[heightmap_shader_index], heightMap_center));
	envLight_index = AddSceneObject(new EnvLight(sceneShaderProgs[envLight_shader_index], heightMap_center), true);
	AddSceneObject(new LakeNode(sceneShaderProgs[lake_shader_index], heightMap_center, sceneCamera, sceneObjects[skybox_index]->getTextureID()));
	policeBox_index = AddSceneObject(new PoliceBox(sceneShaderProgs[policeBox_shader_index], pb_light, lightVol, heightMap_center, false));

	sceneCamera->SetPosition(Vector3(heightMap_center.x, 1500.0f, heightMap_center.z));

	sceneObjects[heightmap_index]->SetShadowCaster(true);
	sceneObjects[policeBox_index]->SetShadowCaster(true);
	sceneObjects[envLight_index]->SetDirectionalLight(true);

	//Perspective matrix to use when drawing the shadow map, the near plane
	//is set closer to the objects been draw to try and maintain higher accuracy 
	//for drawing into the depth buffer. Aspect ratio is set to 1 due to the shadow map being square.
	shadowPersp = Matrix4::Perspective(100.0f, 15000.0f, 1.0f, 45.0f);

	InitialiseLights();

	//UI overlay
	HUD = new UI(SceneNode::context->GetRenderWidth(), SceneNode::context->GetRenderHeight(), true, 
		SceneNode::context->GetPixelPitch(), sceneShaderProgs[UI_shader_index]);

	//Add the text which the UI will draw
	HUD->AddText("Memory Used: " + to_string((Mesh::GetBytesUsed() + OGLRenderer::bytes_used) / (1024 * 1024)) + "(MB)");
	HUD->AddText("Camera Controls: W, A, S, D, Q, E");
	HUD->AddText("Tardis Controls: I, J, K, L, U, O");
	HUD->AddText("Light Controls: Rotate (1, 2), View (P)");
	HUD->AddText("Toggle Wireframe: M");
	HUD->AddText("Toggle Blur: N");

	emitter = new ParticleEmitter(sceneShaderProgs[particle_shader_index]);
}

/*
This function creates point lights with a random colour in a grid of size LIGHTNUM * LIGHTNUM
*/
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
	//Updates the particle emitter.
	emitter->Update(msec);
	//Upates the UI.
	HUD->Update(msec);
	Scene::UpdateScene(msec);	
	
	//Build the view matrix from the lights point of view. The light is positioned high above the heightmap
	//approximately at the center in x,z axes, and then rotated around the heightmap based on user input.
	lightVM = Matrix4::Translation(Vector3(0, 0, -10000.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 0, 1)) *
		Matrix4::Rotation(sceneLightRot, Vector3(0.0f, 1.0f, 0.0f)) *
		Matrix4::BuildViewMatrix(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)) *
		Matrix4::Translation(Vector3(-heightMap_center.x, 0.0f, -heightMap_center.z));

	//Move the light to the correct position, taking the inverse of VM gives a model matrix and from that get the position.
	static_cast<EnvLight*>(sceneObjects[envLight_index])->setPos(Matrix4::Inverse(lightVM).GetPositionVector());
	//Calculate the lights projection, view matrix which is multiplied by a bias matrix to shift the values into texture space (0.0 - 1.0).
	static_cast<EnvLight*>(sceneObjects[envLight_index])->setShadowMatrix(biasMatrix * (shadowPersp * lightVM));
	//The cameras projection, view matrix 
	static_cast<EnvLight*>(sceneObjects[envLight_index])->setShadowProjView(SceneNode::context->GetProjMat() * viewMatrix);
}

void DemoScene::DrawScene(bool shadowPass, bool lightPass)
{	
	if (shadowPass)
	{
		SceneNode::context->UpdateProjMatrix(shadowPersp);

		//The lights view matrix will be used when drawing objects during the shadow pass.
		SceneNode::context->UpdateViewMatrix(lightVM);
	}
	
	Scene::DrawScene(shadowPass, lightPass);

	if (shadowPass)
	{
		//Switch back to a standard projection matrix.
		SceneNode::context->SwitchToPerspective();
		
		//Switch view matrix back to the camera if 'viewLight' is false.
		if (!viewLight)
			SceneNode::context->UpdateViewMatrix(viewMatrix);
	}	
	else if(!lightPass && !blur)
	{
		//Draw particles.
		emitter->Draw();
	}
}

void DemoScene::LateDraw()
{
	//Draw the UI.
	HUD->Draw();
	Scene::LateDraw();
}

void DemoScene::IncrementDirLight(float dt)
{
	sceneLightRot += 2.0f;

	if (sceneLightRot > 360.0f)
		sceneLightRot = 0.0f;

	static_cast<SkyBoxNode*>(sceneObjects[skybox_index])->incAmbient();
}

void DemoScene::DecrementDirLight(float dt) 
{
	sceneLightRot -= 2.0f;

	if (sceneLightRot < 0.0f)
		sceneLightRot = 360.0f;

	static_cast<SkyBoxNode*>(sceneObjects[skybox_index])->decAmbient();
}