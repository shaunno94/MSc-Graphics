/*
Author: Shaun Heald
Holds a simple camera, Vector3 position, yaw, pitch.
Updating the camera polls input provided by the window class.
*/

#pragma once
#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

class Camera 
{
public:
	Camera();
	Camera(float pitch, float yaw, Vector3 position);
	~Camera(){};
	
	//Polls input and updates camera position/rotation accordingly.
	void UpdateCamera(float msec);
	//Returns a view matrix
	Matrix4 BuildViewMatrix();
	
	/* Utility functions, getter/setter's. */
	inline Vector3 GetPosition() const { return position; }
	inline void SetPosition(Vector3 val) { position = val; }

	inline float GetYaw() const { return yaw; }
	inline void SetYaw(float y) { yaw = y; }	

	inline float GetPitch() const { return pitch; }
	inline void SetPitch(float p) { pitch = p; }

protected:
	float yaw;
	float pitch;
	Vector3 position;
};