#pragma once
#include "../nclgl/TextMesh.h"
#include "../nclgl/SceneNode.h"

class UI
{
public:
	UI(int width, int height, bool drawFPS, Vector2 pixSize, Shader* prog);
	~UI();
	void Draw();
	void Update(float msec);
	void AddText(string text);

private:
	const float TEXT_SIZE = 16.0f;

	Font* font;
	TextMesh* fpsText;
	vector<TextMesh*> textarr;
	Vector2 pixSize;
	Matrix4 identity;
	Shader* nodeShader;

	int width, height;
	bool drawFPS;
	float FPS = 0.0f;
	float timePassed = 0.0f;

	unsigned int diffuseTex_loc;
	unsigned int usingTex_loc;
};