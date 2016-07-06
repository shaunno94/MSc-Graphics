#include "Renderer.h"
#include <nclgl\common.h>
#include "DemoScene.h"

#pragma comment(lib, "nclgl.lib")

void ResolveDirectory();

int main() 
{
	ResolveDirectory();

	Window window("Graphics Coursework (Shaun Heald)", 1280, 720, false);
	window.LockMouseToWindow(true);
	window.ShowOSPointer(false);

	if (!window.HasInitialised())
	{
		cout << "Window failed to initialise..." << endl;
		system("pause");
		return -1;
	}

	Renderer renderer(window);

	if (!renderer.HasInitialised())
	{
		cout << "Renderer failed to initialise..." << endl;
		system("pause");
		return -1;
	}

	DemoScene* demo = new DemoScene();
	renderer.SetCurrentScene(demo);

	Vector3 policeBox_pos = Vector3(0,0,0);

	while (window.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
		{
			policeBox_pos.x += 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
		{
			policeBox_pos.x -= 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
		{
			policeBox_pos.z -= 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
		{
			policeBox_pos.z += 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_U))
		{
			policeBox_pos.y += 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
		{
			policeBox_pos.y -= 10.0f;
			//renderer.SetPosition(policeBox_pos);
			demo->SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
		{
			demo->decDirLight();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
		{
			demo->incDirLight();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{
			demo->SwitchViewPoint();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
		{
			renderer.toggleWireFrame();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
		{
			renderer.toggleBlur();
		}
		policeBox_pos.ToZero();
		renderer.RenderScene(window.GetTimer()->GetTimedMS());
	}

	renderer.SetCurrentScene(nullptr);
	delete demo;
	demo = nullptr;
	return 0;
}

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