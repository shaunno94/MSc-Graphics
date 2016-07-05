#include "SkyBoxNode.h"

SkyBoxNode::SkyBoxNode()
{
	mesh = Mesh::GenerateQuad();
	mesh->SetTexture(SOIL_load_OGL_cubemap(SKYBOX_LEFT, SKYBOX_RIGHT,
		SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_FRONT, SKYBOX_BACK, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	OGLRenderer::SetTextureRepeating(mesh->GetTexture(), true);

	if (!mesh->GetTexture())
	{
		cout << "Initialisation failed...Heightmap texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	nodeShader = new Shader(SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER);
	if (!nodeShader->LinkProgram())
	{
		cout << "Initialisation failed...Skybox shader program failed to compile." << endl;
		system("pause");
		exit(1);
	}
	cubeTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "cubeTex");
	ambient_loc = glGetUniformLocation(nodeShader->GetProgram(), "ambient");
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