#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"

class HeightMapNode : public SceneNode
{
public:
	HeightMapNode(Camera* cam, Vector3& hm_center);
	virtual ~HeightMapNode();
	virtual void Update(float msec);
	virtual void DrawNode(bool shadowPass = false);

	inline Vector3 getHMCenter() { return static_cast<HeightMap*>(mesh)->getCenter(); }
	inline Matrix4& getHM_ModelMatrix() { return mesh->getModelMatrix(); }

protected:
	//Textures
	const char* HEIGHTMAP_FILE = "..\\Textures\\Heightmap.jpg";
	const char* HEIGHTMAP_TEX = "..\\Textures\\rockyTex.jpg";
	const char* HEIGHTMAP_BUMP_TEX = "..\\Textures\\Heightmap_NRM.jpg";
	const char* HEIGHTS_NRM = "..\\Textures\\Heights_NRM.jpg";

	//Heightmap shaders
	const char* HEIGHTMAP_FRAG_SHADER = "..\\Shaders\\HM_frag_shader.glsl";
	const char* HEIGHTMAP_VERT_SHADER = "..\\Shaders\\HM_vert_shader.glsl";
	const char* HEIGHTMAP_TCS_SHADER = "..\\Shaders\\HM_TCS_shader.glsl";
	const char* HEIGHTMAP_TES_SHADER = "..\\Shaders\\HM_TES_shader.glsl";

	const float SCALE = 20.0f;

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

	Camera* camera;
};