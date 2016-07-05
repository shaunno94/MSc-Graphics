#pragma once

#include "Plane.h"
#include "SceneNode.h"

class Frustum
{
public:
	Frustum();
	~Frustum();

	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& n);

protected:
	const static unsigned int FRUSTUM_PLANES = 6;
	Plane planes[FRUSTUM_PLANES];
};