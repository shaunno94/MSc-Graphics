#include "PoliceBox.h"

OBJMesh* PoliceBox::policeBox = nullptr;
//http://tf3dm.com/3d-model/tardis-41964.html
const string PoliceBox::PBOX_MESH = "..\\Meshes\\TARDIS.obj";

PoliceBox::PoliceBox(Light*& l, Vector3 center, bool cleanUpMeshOnDelete)
	: SceneNode(nullptr, Vector4(1,1,1,1), cleanUpMeshOnDelete)
{
	Matrix4 modelTranslation = Matrix4::Translation(Vector3(center.x - 400, 1000.0f, center.z - 400));
	Vector3 modelScale = Vector3(90.0f, 90.0f, 90.0f);

	SceneNode* root = new SceneNode(policeBox->GetChild(ROOT), Vector4(0, 0, 1, 1), cleanUpMeshOnDelete);
	root->SetModelScale(modelScale);
	root->SetTransform(modelTranslation);
	root->SetBoundingRadius(1.0f);
	root->GetMesh()->SetTexture(SOIL_load_OGL_texture(TARDIS_TEX, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(root);

	SceneNode* window = new SceneNode(policeBox->GetChild(CHILD_WINDOW), Vector4(0, 0, 0, 0.0f), cleanUpMeshOnDelete);
	window->SetModelScale(modelScale);
	window->SetTransform(modelTranslation);
	window->GetMesh()->SetTexture(SOIL_load_OGL_texture(TARDIS_WINDOW_TEX, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(window);
	
	SceneNode* light = new SceneNode(policeBox->GetChild(PB_CHILD_LIGHT), Vector4(0, 0, 0, 0.0f), cleanUpMeshOnDelete);
	light->SetModelScale(modelScale);
	light->SetTransform(modelTranslation);
	light->GetMesh()->SetTexture(SOIL_load_OGL_texture(TARDIS_LIGHT_TEX, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(light);

	SceneNode* inside = new SceneNode(policeBox->GetChild(CHILD_INSIDE), Vector4(0, 0, 0, 0.0f), cleanUpMeshOnDelete);
	inside->SetModelScale(modelScale);
	inside->SetTransform(modelTranslation);
	inside->GetMesh()->SetTexture(SOIL_load_OGL_texture(TARDIS_INSIDE_TEX, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y));
	AddChild(inside);

	node_IDS[PB_ROOT] = root->getInstanceID();
	node_IDS[WINDOW] = window->getInstanceID();
	node_IDS[PB_LIGHT] = light->getInstanceID();
	node_IDS[INSIDE] = inside->getInstanceID();

	opacity = 1.0f;
	Vector3 tmp = modelTranslation.GetPositionVector();
	tmp.y += 650.0f;

	l = new Light(tmp, Vector3(0,0,0), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 2000.0f, true, 0.6f);
	pb_light = l;

	nodeShader = new Shader(TARDIS_VERT_SHADER, TARDIS_FRAG_SHADER);
	if (!nodeShader->LinkProgram())
	{
		cout << "Initialisation failed...Police box shader program failed to compile." << endl;
		system("pause");
		exit(1);
	}
	nodeColour_loc = glGetUniformLocation(nodeShader->GetProgram(), "nodeColour");
	tardisTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "tardisTex");
	usingTex_loc = glGetUniformLocation(nodeShader->GetProgram(), "usingTex");
	opacity_loc = glGetUniformLocation(nodeShader->GetProgram(), "opacity");
	pb_delta.ToZero();
}

void PoliceBox::CreateBoxInstance()
{
	policeBox = new OBJMesh(PBOX_MESH);
}

void PoliceBox::DeleteBoxInstance()
{
	delete policeBox;
	policeBox = nullptr;
}

void PoliceBox::Update(float msec)
{
	transform = transform * Matrix4::Translation(pb_delta);
	pb_light->setPos(Matrix4::Translation(pb_light->getPos() + pb_delta).GetPositionVector());
	SceneNode::Update(msec);
	pb_delta.ToZero();
}

void PoliceBox::DrawNode(bool shadowPass)
{
	if (!shadowPass)
	{
		context->SetCurrentShader(nodeShader);
	}
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
					glUniform1i(glGetUniformLocation(nodeShader->GetProgram(), "usingTex"), 1);
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