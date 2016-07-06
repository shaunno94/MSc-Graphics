#include "LakeNode.h"

LakeNode::LakeNode(Vector3 offset, Camera* cam, GLuint SB_texID, Light*& l)
{
	mesh = Mesh::GenerateQuad(true);
	mesh->updateType(GL_PATCHES);
	
	mesh->SetTexture(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "water.jpg").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	OGLRenderer::SetTextureRepeating(mesh->GetTexture(), true);

	mesh->SetBumpMap(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "water_NRM.jpg").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	OGLRenderer::SetTextureRepeating(mesh->getBumpMap(), true);	

	water_height = SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "water_height.png").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::SetTextureRepeating(water_height, false);

	water_height2 = SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "water_height2.png").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::SetTextureRepeating(water_height, false);

	if (!mesh->GetTexture() || !mesh->getBumpMap())
	{
		cout << "Initialisation failed...Lake texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	mesh->setModelMatrix(Matrix4::Translation(Vector3(offset.x + 400, 400.0f, offset.z + 400)) *
		Matrix4::Scale(Vector3(offset.x - (offset.x / 2.6f), 1.0f, offset.z - (offset.z / 2.5f))));

	nodeShader = new Shader(File_Locs::SHADER_DIR + "reflect_vert_shader.glsl", File_Locs::SHADER_DIR + "reflect_frag_shader.glsl", 
		File_Locs::SHADER_DIR + "reflect_TCS_shader.glsl", File_Locs::SHADER_DIR + "reflect_TES_shader.glsl");

	if (!nodeShader->LinkProgram())
	{
		cout << "Initialisation failed...Lake shader program failed to compile." << endl;
		system("pause");
		exit(1);
	}

	cameraPos_loc = glGetUniformLocation(nodeShader->GetProgram(), "cameraPos");
	diffuseTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "diffuseTex");
	cubeTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "cubeTex");
	bumpTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "bumpTex"); 
	water_height_loc = glGetUniformLocation(nodeShader->GetProgram(), "waterHeights");
	heightMatrix_loc = glGetUniformLocation(nodeShader->GetProgram(), "heightMatrix");
	water_height2_loc = glGetUniformLocation(nodeShader->GetProgram(), "waterHeights_2");
	heightMatrix2_loc = glGetUniformLocation(nodeShader->GetProgram(), "heightMatrix_2");

	camera = cam;
	skybox_tex = SB_texID;
	envLight = l;
}

LakeNode::~LakeNode()
{}

void LakeNode::DrawNode(bool shadowPass)
{
	if (mesh)
	{
		//glDisable(GL_CULL_FACE);
		context->SetCurrentShader(nodeShader);
		context->SetShaderLight(envLight);

		glUniform3fv(cameraPos_loc, 1, (float*)&camera->GetPosition());
		glUniform1i(diffuseTex_loc, 0);
		glUniform1i(bumpTex_loc, 1);
		glUniform1i(cubeTex_loc, 20);
		glUniform1i(water_height_loc, 21);
		glUniform1i(water_height2_loc, 22);

		glUniformMatrix4fv(heightMatrix_loc, 1, false, (float*)&Matrix4::Rotation(waterRot, Vector3(0, 0, 1)));
		glUniformMatrix4fv(heightMatrix2_loc, 1, false, (float*)&Matrix4::Rotation(waterRot + 100.0f, Vector3(0, 0, 1)));

		glActiveTexture(GL_TEXTURE20);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex);
		glActiveTexture(GL_TEXTURE21);
		glBindTexture(GL_TEXTURE_2D, water_height);
		glActiveTexture(GL_TEXTURE22);
		glBindTexture(GL_TEXTURE_2D, water_height2);

		context->UpdateModelMatrix(mesh->getModelMatrix());
		context->UpdateTextureMatrix(Matrix4::Rotation(waterRot, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)));
		context->UpdateShaderMatrices();

		mesh->Draw();
		//glEnable(GL_CULL_FACE);
	}
}

void LakeNode::Update(float msec)
{
	//SceneNode::Update(msec);
	waterRot += msec / 1000.0f;
	if (waterRot > 360.0f)
		waterRot = 0.0f;
}