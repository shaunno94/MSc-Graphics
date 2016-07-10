/*
Author: Shaun Heald
This class represents a basic text user interface.
*/

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
	//Add a string to be drawn.
	void AddText(string text);

private:
	const float TEXT_SIZE = 16.0f;

	Font* font;
	//Seperate mesh for the FPS counter beacuse it need to be re-created after an update.
	TextMesh* fpsText;
	//Holds all strings that will be drawn by this object.
	vector<TextMesh*> textarr;
	Vector2 pixSize;
	Matrix4 identity;
	Shader* nodeShader;

	//Width & height of the font
	int width, height;
	//Whether or not the FPS counter is drawn.
	bool drawFPS;
	float FPS = 0.0f;
	//Time since last update of fps counter.
	float timePassed = 0.0f;

	unsigned int diffuseTex_loc;
	unsigned int usingTex_loc;
};