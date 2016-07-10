/*
Author: Shaun Heald
Extends scene node class and represents a PoliceBox object.
Note: CreateBoxInstance should be called before using this class, this ensures
the OBJ file has been loaded into memory. When finished DeleteBoxInstance should be called to release the OBJ data.
*/
#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/ObjMesh.h"

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
	PoliceBox(Shader* shader, Light*& l, Mesh* lightVol, Vector3 center, bool cleanUpMeshOnDelete);
	virtual ~PoliceBox();

	//Update the node.
	virtual void Update(float msec);
	//Draw this node.
	virtual void DrawNode(bool shadowPass = false);

	/* Instantiate object mesh only once, to prevent loading from file multiple times. */
	static void CreateBoxInstance();
	/* Destroy object mesh. */
	static void DeleteBoxInstance();

protected:
	static OBJMesh* policeBox;
	void UpdateOpacity(float dt);
	unsigned int node_IDS[PB_MAX_ID];

	GLuint tardisTex_loc;
	GLuint usingTex_loc;
	GLuint nodeColour_loc;
	GLuint opacity_loc;

	Vector3 pb_delta = Vector3(0, 0, 0);

	float opacity;
	bool opaque = true;
	Light* pb_light;
};
