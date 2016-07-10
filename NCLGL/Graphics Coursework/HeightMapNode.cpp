#include "HeightMapNode.h"

HeightMapNode::HeightMapNode(Shader* shader, Vector3& hm_center)
{
	//Create a new heightmap by loading data from file.
	mesh = new HeightMap((File_Locs::TEXTURE_DIR + "Heightmap.jpg").c_str(), (File_Locs::TEXTURE_DIR + "Heights_NRM.jpg").c_str(),
		SCALE, (File_Locs::TEXTURE_DIR + "rockyTex.jpg").c_str(), (File_Locs::TEXTURE_DIR + "Heightmap_NRM.jpg").c_str());
	
	if (!mesh->GetTexture() || !mesh->GetBumpMap())
	{
		cout << "Initialisation failed...Heightmap texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	OGLRenderer::SetTextureFiltering(mesh->GetTexture(), true, true);
	OGLRenderer::SetTextureRepeating(mesh->GetTexture(), true);

	hm_center = static_cast<HeightMap*>(mesh)->getCenter();
	
	nodeShader = shader;
	deleteMesh = true;

	/* Shader uniform locations. */
	heightMapTex_loc = nodeShader->GetUniformLocation("rockTex");
	heights_loc = nodeShader->GetUniformLocation("heights");
	modelMat_loc = nodeShader->GetUniformLocation("modelMatrix");
	snowTex_loc = nodeShader->GetUniformLocation("snowTex");
	grassTex_loc = nodeShader->GetUniformLocation("grassTex");
	bump_loc = nodeShader->GetUniformLocation("bumpTex");
	heights_nrm_loc = nodeShader->GetUniformLocation("heights_nrm");
	tes_inner_loc = nodeShader->GetUniformLocation("innerLevel");
	tes_outer_loc = nodeShader->GetUniformLocation("outerLevel");
}

HeightMapNode::~HeightMapNode()
{}

void HeightMapNode::Update(float msec)
{
	SceneNode::Update(msec);
}

/* Set OpenGL state, update shader uniforms, update matrices and draw this node. */
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

		context->UpdateModelMatrix(mesh->getModelMatrix());
		context->UpdateShaderMatrices();
		mesh->Draw();
		glUseProgram(0);
	}
}