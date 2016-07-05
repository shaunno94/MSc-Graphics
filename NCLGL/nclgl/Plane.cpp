#include "Plane.h"

Plane::Plane(){}
Plane::~Plane(){}

Plane::Plane(const Vector3& normal, float dist, bool normalise)
{
	if (normalise)
	{
		float len = sqrt(Vector3::Dot(normal, normal));
		this->normal = normal / len;
		this->dist = dist / len;
	}
	else
	{
		this->normal = normal;
		this->dist = dist;
	}
}

bool Plane::SphereInPlane(const Vector3& position, float radius) const
{
	/*if (Vector3::Dot(position, normal) + dist <= -radius)
	{
		return false;
	}
	return true;*/
	return !((Vector3::Dot(position, normal) + dist) <= -radius);
}