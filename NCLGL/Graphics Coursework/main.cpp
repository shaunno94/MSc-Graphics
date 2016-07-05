#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() 
{
	Window w("Graphics Coursework (Shaun Heald)", 1280, 720, false);

	if (!w.HasInitialised())
	{
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised())
	{
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	Vector3 policeBox_pos = Vector3(0,0,0);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
		{
			policeBox_pos.x += 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
		{
			policeBox_pos.x -= 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
		{
			policeBox_pos.z -= 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
		{
			policeBox_pos.z += 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_U))
		{
			policeBox_pos.y += 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
		{
			policeBox_pos.y -= 10.0f;
			renderer.SetPosition(policeBox_pos);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
		{
			renderer.decDirLight();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
		{
			renderer.incDirLight();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{
			renderer.switchViewPoint();
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
		renderer.RenderScene(w.GetTimer()->GetTimedMS());
	}
	return 0;
}