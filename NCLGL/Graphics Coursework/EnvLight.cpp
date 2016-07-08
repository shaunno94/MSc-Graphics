#include "EnvLight.h"

EnvLight::EnvLight(Shader* shader, Vector3 pos)
	: SceneNode(Mesh::GenerateQuad(), Vector4(1, 1, 1, 1), shader, true)
{
	dirLight = new Light(Vector3(pos.x, 0.0f, pos.z), Vector3(pos.x, 1000.0f, pos.z), lightCol, 1.0f, false, 1.0f, nullptr);
	lightRot = 0.0f;
	
	dir_loc = nodeShader->GetUniformLocation("dir");
	shadowVP_loc = nodeShader->GetUniformLocation("shadowViewProj");
	shadowMatrix_loc = nodeShader->GetUniformLocation("shadowMatrix");
}

EnvLight::~EnvLight()
{
	delete dirLight;
	dirLight = nullptr;
}

void EnvLight::DrawNode(bool shadowPass)
{
	glDisable(GL_CULL_FACE);
	context->SwitchToOrtho();

	glUniform1i(dir_loc, 1);
	glUniformMatrix4fv(shadowVP_loc, 1, false, (float*)&Matrix4::Inverse(shadowVP));
	glUniformMatrix4fv(shadowMatrix_loc, 1, false, (float*)&shadowProj);

	context->SetShaderLight(dirLight);
	context->UpdateShaderMatrices();

	mesh->Draw();
	glEnable(GL_CULL_FACE);
	context->SwitchToPerspective();
}

void EnvLight::Update(float msec)
{

}