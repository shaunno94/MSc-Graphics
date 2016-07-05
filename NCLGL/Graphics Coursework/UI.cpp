#include "UI.h"

UI::UI(int width, int height, unsigned int bytes_used, Vector2 pixSize, Shader* prog)
{
	int conv = int(pow(1024, 2));
	this->pixSize = pixSize;
	font = new Font(SOIL_load_OGL_texture(FONT_FILE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	textarr.push_back(new TextMesh("Memory Used: " + to_string((Mesh::GetBytesUsed() + bytes_used) / conv) + "(MB)", *font));
	textarr.push_back(new TextMesh("Camera Controls: W, A, S, D, Q, E", *font));
	textarr.push_back(new TextMesh("Tardis Controls: I, J, K, L, U, O", *font));
	textarr.push_back(new TextMesh("Light Controls: Rotate (1, 2), View (P)", *font));
	textarr.push_back(new TextMesh("Toggle Wireframe: M", *font));
	textarr.push_back(new TextMesh("Toggle Blur: N", *font));
	fpsText = nullptr;
	nodeShader = prog;
	this->width = width;
	this->height = height;
}

UI::~UI()
{
	for (unsigned int i = 0; i < textarr.size(); ++i)
	{
		delete textarr[i];
	}

	delete fpsText;
	delete font;
	font = nullptr;
	fpsText = nullptr;
}

void UI::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	SceneNode::context->SetCurrentShader(nodeShader);
	glUniform1i(glGetUniformLocation(nodeShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(nodeShader->GetProgram(), "font"), 1);

	if (!fpsText || timePassed > 500.0f)
	{
		timePassed = 0.0f;
		fpsText = new TextMesh("FPS: " + to_string(FPS), *font);
	}

	SceneNode::context->UpdateModelMatrix(Matrix4::Translation(Vector3(0, height, 0)) * Matrix4::Scale(Vector3(TEXT_SIZE, TEXT_SIZE, 1)));
	SceneNode::context->UpdateViewMatrix(identity);
	SceneNode::context->UpdateProjMatrix(Matrix4::Orthographic(-1.0f, 1.0f, float(width), 0.0f, float(height), 0.0f));
	SceneNode::context->UpdateShaderMatrices();

	fpsText->Draw();

	for (unsigned int i = 0; i < textarr.size(); ++i)
	{
		SceneNode::context->UpdateModelMatrix(Matrix4::Translation(Vector3(0, height - ((i + 1) * 30), 0)) * Matrix4::Scale(Vector3(TEXT_SIZE, TEXT_SIZE, 1)));
		SceneNode::context->UpdateShaderMatrices();
		textarr[i]->Draw();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UI::Update(float msec)
{
	FPS = 1000.0f / msec;
	timePassed += msec;
}