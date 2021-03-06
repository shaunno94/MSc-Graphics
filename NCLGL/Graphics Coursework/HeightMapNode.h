/*
Author: Shaun Heald
This class extends Scene Node and represents the terrain in the demo scene.
This class creates a heightmap mesh.
*/

#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"

class HeightMapNode : public SceneNode
{
public:
	HeightMapNode(Shader* shader, Vector3& hm_center);
	virtual ~HeightMapNode();
	virtual void Update(float msec);
	virtual void DrawNode(bool shadowPass = false);

	inline Vector3 getHMCenter() const	{ return static_cast<HeightMap*>(mesh)->getCenter(); }
	inline Matrix4& getHM_ModelMatrix() { return mesh->getModelMatrix(); }

protected:	
	const float SCALE = 20.0f;

	/* Shader uniform handles. */
	GLuint heightMapTex_loc;
	GLuint heights_loc;
	GLuint bump_loc;
	GLuint snowTex_loc;
	GLuint grassTex_loc;
	GLuint modelMat_loc;
	GLuint cameraPos_loc;
	GLuint heights_nrm_loc;
	GLuint tes_outer_loc;
	GLuint tes_inner_loc;
};