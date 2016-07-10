/*
Author: Shaun Heald
Main entry point to program.
*/

#include "Renderer.h"
#include <nclgl\common.h>
#include "DemoScene.h"

#pragma comment(lib, "nclgl.lib")

void ResolveDirectory();

int main() 
{
	ResolveDirectory();

	//Create the window for the OpenGL context.
	Window window("Graphics Coursework (Shaun Heald)", 1280, 720, false);
	window.LockMouseToWindow(true);
	window.ShowOSPointer(false);

	if (!window.HasInitialised())
	{
		cout << "Window failed to initialise..." << endl;
		system("pause");
		return -1;
	}

	//Create the renderer context.
	Renderer renderer(window);

	if (!renderer.HasInitialised())
	{
		cout << "Renderer failed to initialise..." << endl;
		system("pause");
		return -1;
	}

	//Initialise a scene and pass it to the renderer for drawing.
	DemoScene* demo = new DemoScene();
	renderer.SetCurrentScene(demo);

	float dt = 0.001f;

	//Main game loop.
	while (window.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		dt = window.GetTimer()->GetTimedMS();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1) || Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD1))
		{
			demo->DecrementDirLight(dt);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2) || Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD2))
		{
			demo->IncrementDirLight(dt);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{
			demo->SwitchViewPoint();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
		{
			renderer.ToggleWireFrame();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
		{
			renderer.ToggleBlur();
			demo->toggleBlur();
		}
		renderer.RenderScene(dt);
	}

	renderer.SetCurrentScene(nullptr);
	delete demo;
	demo = nullptr;
	return 0;
}

//Get the path to the texture, shader and mesh folders.
void ResolveDirectory()
{
	string topDir = "NCLGL\\";
	string shaderDir = "Shaders\\";
	string texDir = "Textures\\";
	string meshDir = "Meshes\\";
	char raw[1024];

	GetModuleFileName(NULL, raw, 1024);

	string fileName = raw;
	unsigned int index = fileName.find(topDir);
	index += topDir.length();
	
	File_Locs::SHADER_DIR = fileName.replace(fileName.begin() + index, fileName.end(), shaderDir);
	File_Locs::TEXTURE_DIR = fileName.replace(fileName.begin() + index, fileName.end(), texDir);
	File_Locs::MESH_DIR = fileName.replace(fileName.begin() + index, fileName.end(), meshDir);
}