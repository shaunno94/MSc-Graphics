#include "UI.h"

UI::UI(int width, int height, bool drawFPS, Vector2 pixSize, Shader* prog)
{
	this->pixSize = pixSize;

	//Load the font texture.
	font = new Font(SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "tahoma.tga").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	
	if (!font)
	{
		cout << "Initialisation failed...Font texture failed to load." << endl;
		system("pause");
		exit(1);
	}

	if(drawFPS)
		fpsText = new TextMesh("FPS: " + to_string(FPS), *font);

	nodeShader = prog;
	this->width = width;
	this->height = height;
	this->drawFPS = drawFPS;

	/* Shader uniforms. */
	diffuseTex_loc = nodeShader->GetUniformLocation("diffuseTex");
	usingTex_loc = nodeShader->GetUniformLocation("usingTexture");
}

UI::~UI()
{
	for (unsigned int i = 0; i < textarr.size(); ++i)
	{
		delete textarr[i];
	}
	
	if (fpsText)
	{
		delete fpsText;
		fpsText = nullptr;
	}
	
	delete font;
	font = nullptr;
	textarr.clear();
}

void UI::AddText(string text)
{
	textarr.push_back(new TextMesh(text, *font));
}

/* Set OpenGL state, update shader uniforms, update matrices and draw this node. */
void UI::Draw()
{
	SceneNode::context->SetCurrentShader(nodeShader);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	
	glUniform1i(diffuseTex_loc, 0);
	glUniform1i(usingTex_loc, 1);

	//Create a new mesh for the FPS counter after an interval of 500ms.
	if (timePassed > 500.0f && drawFPS)
	{
		timePassed = 0.0f;
		delete fpsText;
		fpsText = new TextMesh("FPS: " + to_string(FPS).substr(0, 4), *font);
	}

	SceneNode::context->UpdateModelMatrix(Matrix4::Translation(Vector3(0, height, 0)) * Matrix4::Scale(Vector3(TEXT_SIZE, TEXT_SIZE, 1)));
	SceneNode::context->UpdateViewMatrix(identity);
	SceneNode::context->UpdateProjMatrix(Matrix4::Orthographic(-1.0f, 1.0f, float(width), 0.0f, float(height), 0.0f));
	SceneNode::context->UpdateShaderMatrices();

	if(drawFPS)
		fpsText->Draw();

	//Loop through text array, translating down in the Y for each line.
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
	FPS = (1000.0f / msec);
	timePassed += msec;
}