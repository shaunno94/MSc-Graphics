#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/ObjMesh.h"
#include "../nclgl/MD5Mesh.h"

enum PB_ID
{
	PB_ROOT, WINDOW, PB_LIGHT, INSIDE, PB_MAX_ID
};

enum PB_CHILDREN
{
	CHILD_INSIDE = 0, ROOT = 3, CHILD_WINDOW = 4, PB_CHILD_LIGHT = 2
};

class PoliceBox : public SceneNode
{
public:
	PoliceBox(Light*& l, Vector3 center, bool cleanUpMeshOnDelete);
	virtual ~PoliceBox();

	virtual void Update(float msec);
	virtual void DrawNode(bool shadowPass = false);
	static void CreateBoxInstance();
	static void DeleteBoxInstance();

	inline int getNodeID(enum PB_ID p_id) { return node_IDS[p_id]; }
	inline SceneNode* getObjectRoot() { return children[ROOT]; }
	inline float getOpacity() { incOpacity(); return opacity; }
	inline void updatePos(Vector3& v){ pb_delta = v; }

protected:
	static OBJMesh* policeBox;
	static const string PBOX_MESH;
	void incOpacity();
	unsigned int node_IDS[PB_MAX_ID];

	const char* TARDIS_TEX = "..\\Textures\\TARDIS_D.tga";
	const char* TARDIS_WINDOW_TEX = "..\\Textures\\TARDIS_Glass1_D.tga";
	const char* TARDIS_LIGHT_TEX = "..\\Textures\\TARDIS_Bulb_D.tga";
	const char* TARDIS_INSIDE_TEX = "..\\Textures\\TARDIS_inside_D.tga";
	//Tardis police box shader
	const char* TARDIS_VERT_SHADER = "..\\Shaders\\PB_vert_shader.glsl";
	const char* TARDIS_FRAG_SHADER = "..\\Shaders\\PB_frag_shader.glsl";

	GLuint tardisTex_loc;
	GLuint usingTex_loc;
	GLuint nodeColour_loc;
	GLuint opacity_loc;

	Vector3 pb_delta = Vector3(0, 0, 0);

	float opacity;
	bool opaque = true;
	const unsigned int SHADOW_SIZE = 2048;
	Light* pb_light;
};
