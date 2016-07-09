#include "PoliceBox.h"

OBJMesh* PoliceBox::policeBox = nullptr;

PoliceBox::PoliceBox(Shader* shader, Light*& l, Mesh* lightVol, Vector3 center, bool cleanUpMeshOnDelete)
	: SceneNode(nullptr, Vector4(1,1,1,1), shader, cleanUpMeshOnDelete)
{
	Matrix4 modelTranslation = Matrix4::Translation(Vector3(center.x - 400, 1000.0f, center.z - 400));
	Vector3 modelScale = Vector3(90.0f, 90.0f, 90.0f);

	SceneNode* root = new SceneNode(policeBox->GetChild(ROOT), Vector4(0, 0, 1, 1), shader, cleanUpMeshOnDelete);
	root->SetModelScale(modelScale);
	root->SetTransform(modelTranslation);
	root->SetBoundingRadius(1.0f);
	root->GetMesh()->SetTexture(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + ("TARDIS_D.tga")).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(root);

	SceneNode* window = new SceneNode(policeBox->GetChild(CHILD_WINDOW), Vector4(0, 0, 0, 0.0f), shader, cleanUpMeshOnDelete);
	window->SetModelScale(modelScale);
	window->SetTransform(modelTranslation);
	window->GetMesh()->SetTexture(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + ("TARDIS_Glass1_D.tga")).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(window);
	
	SceneNode* light = new SceneNode(policeBox->GetChild(PB_CHILD_LIGHT), Vector4(0, 0, 0, 0.0f), shader, cleanUpMeshOnDelete);
	light->SetModelScale(modelScale);
	light->SetTransform(modelTranslation);
	light->GetMesh()->SetTexture(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + ("TARDIS_Bulb_D.tga")).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(light);

	SceneNode* inside = new SceneNode(policeBox->GetChild(CHILD_INSIDE), Vector4(0, 0, 0, 0.0f), shader, cleanUpMeshOnDelete);
	inside->SetModelScale(modelScale);
	inside->SetTransform(modelTranslation);
	inside->GetMesh()->SetTexture(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + ("TARDIS_inside_D.tga")).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(inside);

	node_IDS[PB_ROOT] = root->getInstanceID();
	node_IDS[WINDOW] = window->getInstanceID();
	node_IDS[PB_LIGHT] = light->getInstanceID();
	node_IDS[INSIDE] = inside->getInstanceID();

	opacity = 1.0f;
	Vector3 tmp = modelTranslation.GetPositionVector();
	tmp.y += 650.0f;

	l = new Light(tmp, Vector3(0, 0, 0), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 2000.0f, true, 0.6f, lightVol);
	pb_light = l;

	nodeColour_loc = nodeShader->GetUniformLocation("nodeColour");
	tardisTex_loc = nodeShader->GetUniformLocation("tardisTex");
	usingTex_loc = nodeShader->GetUniformLocation("usingTex");
	opacity_loc = nodeShader->GetUniformLocation("opacity");
	pb_delta.ToZero();
}

void PoliceBox::CreateBoxInstance()
{
	if (!policeBox)
	{
		//Mesh from: http://tf3dm.com/3d-model/tardis-41964.html
		policeBox = new OBJMesh(File_Locs::MESH_DIR + "TARDIS.obj");
	}
}

void PoliceBox::DeleteBoxInstance()
{
	if (policeBox)
	{
		delete policeBox;
		policeBox = nullptr;
	}
}

void PoliceBox::Update(float msec)
{
	transform = transform * Matrix4::Translation(pb_delta);
	pb_light->setPos(Matrix4::Translation(pb_light->getPos() + (pb_delta / msec)).GetPositionVector());
	SceneNode::Update(msec);
	pb_delta.ToZero();
}

void PoliceBox::DrawNode(bool shadowPass)
{
	context->SetCurrentShader(nodeShader);

	for (unsigned int i = 0; i < children.size(); ++i)
	{
		if (children[i]->GetMesh())
		{	
			if (shadowPass)
			{
				if (children[i]->getInstanceID() == node_IDS[INSIDE] || children[i]->getInstanceID() == node_IDS[WINDOW])
					return;
			}
			else
			{
				if (children[i]->getInstanceID() == node_IDS[PB_LIGHT])
				{
					glUniform1i(usingTex_loc, 0);
					glUniform4fv(nodeColour_loc, 1, (float*)&Vector4(opacity, 0, 0, 1));
				}
				else
				{
					glUniform1i(tardisTex_loc, 0);
					glUniform1i(usingTex_loc, 1);
				}
				glUniform1f(opacity_loc, getOpacity());
			}

			context->UpdateModelMatrix(children[i]->GetMesh()->getModelMatrix());
			context->UpdateShaderMatrices();
			children[i]->GetMesh()->Draw();
		}
		children[i]->DrawNode(shadowPass);
	}
	glUseProgram(0);
}

void PoliceBox::incOpacity()
{
	opaque ? opacity -= 0.002f : opacity += 0.002f;
	if (opacity > 1.0f)
	{
		opaque = true;
	}
	else if (opacity < 0.0f)
	{
		opaque = false;
	}
	pb_light->setColour(Vector4(opacity, 0, 0, 1));
}

PoliceBox::~PoliceBox()
{}