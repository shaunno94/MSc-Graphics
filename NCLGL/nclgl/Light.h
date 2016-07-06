#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Mesh.h"

class Light
{
public:
	Light(Vector3 pos, Vector3 offset, Vector4 col, float rad, bool Static, float inty, Mesh* lightVol) 
	{
		position = pos; colour = col; 
		radius = rad; isStatic = Static;
		intensity = inty;
		this->offset = offset;
		this->lightVol = lightVol;
	}
	~Light(){}

	Vector3& getPos() { return position; }
	inline void setPos(Vector3& v) { position = v; }

	inline float getRadius() const { return radius; }
	inline void setRadius(float f) { radius = f; }

	inline Vector4 getColour() const { return colour; }
	inline void setColour(Vector4& v) { colour = v; }

	inline bool getIsStatic() const { return isStatic; }
	inline float getIntensity() const { return intensity; }
	inline void setIntensity(float f) { intensity = f; }
	inline Vector3 getOffset() const { return offset; }

	inline void Draw() { 
		if (lightVol) 
			lightVol->Draw(); 
	}

protected:
	Vector3 position;
	Mesh* lightVol;
	Vector3 offset;
	Vector4 colour;
	float radius;
	bool isStatic;
	float intensity;
};