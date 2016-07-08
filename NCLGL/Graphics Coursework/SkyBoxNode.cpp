#include "SkyBoxNode.h"

SkyBoxNode::SkyBoxNode(Shader* shader)
	: SceneNode(Mesh::GenerateQuad(), Vector4(1, 1, 1, 1), shader, true)
{
	//mesh = Mesh::GenerateQuad();
	//Skybox texture - cube map: left, right, top, bottom, front, back
	mesh->SetTexture(SOIL_load_OGL_cubemap((File_Locs::TEXTURE_DIR + "ThickCloudsWaterLeft.png").c_str(), 
		(File_Locs::TEXTURE_DIR + ("ThickCloudsWaterRight.png")).c_str(),
		(File_Locs::TEXTURE_DIR + ("ThickCloudsWaterUp.png")).c_str(),
		(File_Locs::TEXTURE_DIR + ("ThickCloudsWaterDown.png")).c_str(),
		(File_Locs::TEXTURE_DIR + ("ThickCloudsWaterFront.png")).c_str(),
		(File_Locs::TEXTURE_DIR + ("ThickCloudsWaterBack.png")).c_str(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	OGLRenderer::SetTextureRepeating(mesh->GetTexture(), true);

	if (!mesh->GetTexture())
	{
		cout << "Initialisation failed...Skybox texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	cubeTex_loc = nodeShader->GetUniformLocation("cubeTex");
	ambient_loc = nodeShader->GetUniformLocation("ambient");
}

SkyBoxNode::~SkyBoxNode()
{}

void SkyBoxNode::Update(float msec)
{
	SceneNode::Update(msec);
}

void SkyBoxNode::DrawNode(bool shadowPass)
{
	if (mesh)
	{
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		context->SetCurrentShader(nodeShader);
		context->UpdateShaderMatrices();

		glUniform1i(cubeTex_loc, 0);
		glUniform1f(ambient_loc, ambient);
		mesh->Draw();

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glUseProgram(0);
	}
}