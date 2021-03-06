#include "Camera.h"

Camera::Camera()
{
	position.ToZero();
	yaw = 0.0f;
	pitch = 0.0f;
}

Camera::Camera(float pitch, float yaw, Vector3 position)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->position = position;
}

void Camera::UpdateCamera(float msec)
{
	//Poll mouse
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);
	
	//Lock pitch between -90, 90
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0.0f) 
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f) 
	{
		yaw -= 360.0f;
	}

	/* Poll keyboard */
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
	{
		position += Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, -1.0f) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
	{
		position -= Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, -1.0f) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
	{
		position += Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(-1.0f, 0.0f, 0.0f) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
	{
		position -= Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(-1.0f, 0.0f, 0.0f) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q))
	{
		position.y += msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E))
	{
		position.y -= msec;
	}
}

Matrix4 Camera::BuildViewMatrix()
{
	//View matrix is just a inverted model matrix.
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}