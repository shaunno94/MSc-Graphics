#pragma once
#include "Vector3.h"

class Plane
{
public:
	Plane();
	Plane(const Vector3& normal, float dist, bool normalise = false);
	~Plane();

	inline void SetNormal(const Vector3& n){ normal = n; }
	inline Vector3 GetNormal() const { return normal; }
	inline void SetDistance(float d) { dist = d; }
	inline float GetDistance() const { return dist; }

	bool SphereInPlane(const Vector3& position, float radius) const;

protected:
	Vector3 normal;
	float dist;
};