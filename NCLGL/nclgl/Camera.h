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
	
	void UpdateCamera(float msec = 10.0f);
	Matrix4 BuildViewMatrix();
	
	inline Vector3 GetPosition() const { return position; }
	inline void SetPosition(Vector3 val) { position = val; }
	
	inline float GetYaw() const { return yaw; }
	inline void SetYaw(float y) { yaw = y; }
	
	inline float GetPitch() const { return pitch; }
	inline void SetPitch(float p) { pitch = p; }

protected:
	float yaw;
	float pitch;
	Vector3 position; // Set to 0,0,0 by Vector3 constructor
};