#include "Light.h"
#include "Mesh.h"

Light::Light(Vector3 position, Vector3 offset, Vector4 colour, float radius, bool isStatic, float intensity, Mesh* lightVol)
{
	this->position = position;
	this->colour = colour;
	this->radius = radius;
	this->isStatic = isStatic;
	this->intensity = intensity;
	this->offset = offset;
	this->lightVol = lightVol;
}

void Light::Draw() 
{
	if (lightVol)
		lightVol->Draw();
}