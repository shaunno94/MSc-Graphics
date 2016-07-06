#pragma once
#include "../nclgl/TextMesh.h"
#include "../nclgl/SceneNode.h"

class UI
{
public:
	UI(int width, int height, unsigned int bytes_used, Vector2 pixSize, Shader* prog);
	~UI();
	void Draw();
	void Update(float msec);

private:
	const float TEXT_SIZE = 16.0f;

	Font* font;
	TextMesh* fpsText;
	vector<TextMesh*> textarr;
	Vector2 pixSize;
	Matrix4 identity;
	Shader* nodeShader;

	int width, height;
	int FPS = 0;
	float timePassed = 0.0f;
};