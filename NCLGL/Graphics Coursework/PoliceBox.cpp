#include "PoliceBox.h"
#include <math.h>
OBJMesh* PoliceBox::policeBox = nullptr;

PoliceBox::PoliceBox(Shader* shader, Light*& l, Mesh* lightVol, Vector3 center, bool cleanUpMeshOnDelete)
	: SceneNode(nullptr, Vector4(1,1,1,1), shader, cleanUpMeshOnDelete)
{
	Matrix4 modelTranslation = Matrix4::Translation(Vector3(center.x - 400.0f, 1000.0f, center.z - 400.0f));
	Vector3 modelScale = Vector3(90.0f, 90.0f, 90.0f);

	/* Initialise the objects component parts. */
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

	/* Setup the light on top of the police box. */
	opacity = 1.0f;
	Vector3 tmp = modelTranslation.GetPositionVector();
	tmp.y += 650.0f;

	l = new Light(tmp, Vector3(0, 0, 0), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 2000.0f, true, 0.6f, lightVol);
	pb_light = l;

	/* Shader uniform locations. */
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
	/* Movement controls for the police box object. */
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
	{
		pb_delta.x += 10.0f;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
	{
		pb_delta.x -= 10.0f;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
	{
		pb_delta.z -= 10.0f;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
	{
		pb_delta.z += 10.0f;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_U))
	{
		pb_delta.y += 10.0f;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
	{
		pb_delta.y -= 10.0f;
	}

	/* Move the object. */
	pb_delta / msec;
	transform = transform * Matrix4::Translation(pb_delta);
	pb_light->setPos(Matrix4::Translation(pb_light->getPos() + pb_delta).GetPositionVector());

	pb_delta.ToZero();
	UpdateOpacity(msec);

	SceneNode::Update(msec);
}

/* Set OpenGL state, update shader uniforms, update matrices and draw this node. */
void PoliceBox::DrawNode(bool shadowPass)
{
	context->SetCurrentShader(nodeShader);

	for (unsigned int i = 0; i < children.size(); ++i)
	{
		if (children[i]->GetMesh())
		{	
			if (shadowPass)
			{
				//Dont render these parts of the box to the shadow map.
				if (children[i]->getInstanceID() == node_IDS[INSIDE] || children[i]->getInstanceID() == node_IDS[WINDOW])
					return;
			}
			else
			{
				if (children[i]->getInstanceID() == node_IDS[PB_LIGHT])
				{
					glUniform1i(usingTex_loc, 0);
					glUniform4fv(nodeColour_loc, 1, (float*)&Vector4(max(0, opacity), 0, 0, 1));
				}
				else
				{
					glUniform1i(tardisTex_loc, 0);
					glUniform1i(usingTex_loc, 1);
				}
				glUniform1f(opacity_loc, max(0, opacity));
			}

			context->UpdateModelMatrix(children[i]->GetMesh()->getModelMatrix());
			context->UpdateShaderMatrices();
			children[i]->GetMesh()->Draw();
		}
		children[i]->DrawNode(shadowPass);
	}
	glUseProgram(0);
}

/* Update the objects alpha and light colour. */
void PoliceBox::UpdateOpacity(float dt)
{
	opaque ? opacity -= 0.1f / dt : opacity += 0.1f / dt;

	//Quick way to hold to alpha of the object to 1 or 0 for more than a single frame.
	if (opacity > 1.5f)
		opaque = true;
	else if (opacity < -0.5f)
		opaque = false;

	//Set the light colour R value
	pb_light->setColour(Vector4(max(0, opacity), 0, 0, 1));
}

PoliceBox::~PoliceBox()
{}