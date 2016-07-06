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
	PoliceBox(Shader* shader, Light*& l, Vector3 center, bool cleanUpMeshOnDelete);
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
	void incOpacity();
	unsigned int node_IDS[PB_MAX_ID];

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
