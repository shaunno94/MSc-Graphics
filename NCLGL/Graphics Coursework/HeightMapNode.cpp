#include "HeightMapNode.h"

HeightMapNode::HeightMapNode(Camera* cam, Vector3& hm_center)
{
	mesh = new HeightMap(HEIGHTMAP_FILE, HEIGHTS_NRM, SCALE, HEIGHTMAP_TEX, HEIGHTMAP_BUMP_TEX);
	OGLRenderer::SetTextureFiltering(mesh->GetTexture(), true, true);
	OGLRenderer::SetTextureRepeating(mesh->GetTexture(), true);
	if (!mesh->GetTexture() || !mesh->getBumpMap())
	{
		cout << "Initialisation failed...Heightmap texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	camera = cam;

	hm_center = static_cast<HeightMap*>(mesh)->getCenter();

	nodeShader = new Shader(HEIGHTMAP_VERT_SHADER, HEIGHTMAP_FRAG_SHADER, HEIGHTMAP_TCS_SHADER, HEIGHTMAP_TES_SHADER);
	if (!nodeShader->LinkProgram())
	{
		cout << "Initialisation failed...Heightmap shader program failed to compile." << endl;
		system("pause");
		exit(1);
	}
	heightMapTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "rockTex");
	heights_loc = glGetUniformLocation(nodeShader->GetProgram(), "heights");
	modelMat_loc = glGetUniformLocation(nodeShader->GetProgram(), "modelMatrix");
	cameraPos_loc = glGetUniformLocation(nodeShader->GetProgram(), "cameraPos");
	snowTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "snowTex");
	grassTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "grassTex");
	bump_loc = glGetUniformLocation(nodeShader->GetProgram(), "bumpTex");
	heights_nrm_loc = glGetUniformLocation(nodeShader->GetProgram(), "heights_nrm");
	tes_inner_loc = glGetUniformLocation(nodeShader->GetProgram(), "innerLevel");
	tes_outer_loc = glGetUniformLocation(nodeShader->GetProgram(), "outerLevel");
}

HeightMapNode::~HeightMapNode()
{}

void HeightMapNode::Update(float msec)
{
	SceneNode::Update(msec);
}

void HeightMapNode::DrawNode(bool shadowPass)
{
	if (mesh)
	{
		context->SetCurrentShader(nodeShader);
		if (shadowPass)
		{
			glUniform1i(tes_inner_loc, 1);
			glUniform1i(tes_outer_loc, 1);
		}
		else
		{
			glUniform1i(tes_inner_loc, 5);
			glUniform1i(tes_outer_loc, 5);
		}
		glUniform1i(heightMapTex_loc, 0);
		glUniform1i(bump_loc, 1);
		glUniform1i(heights_loc, 2);
		glUniform1i(snowTex_loc, 10);
		glUniform1i(grassTex_loc, 11);
		glUniform1i(heights_nrm_loc, 12);
		glUniform3fv(cameraPos_loc, 1, (float*)&camera->GetPosition());
		context->UpdateModelMatrix(mesh->getModelMatrix());
		context->UpdateShaderMatrices();
		mesh->Draw();
		glUseProgram(0);
	}
}