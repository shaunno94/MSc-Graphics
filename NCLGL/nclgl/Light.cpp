#include "Light.h"
#include "Mesh.h"

Light::Light(Vector3 pos, Vector3 offset, Vector4 col, float rad, bool Static, float inty, Mesh* lightVol)
{
	position = pos; colour = col;
	radius = rad; isStatic = Static;
	intensity = inty;
	this->offset = offset;
	this->lightVol = lightVol;
}

void Light::Draw() 
{
	if (lightVol)
		lightVol->Draw();
}