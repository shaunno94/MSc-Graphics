/*
Author: Shaun Heald
Base Scene Node Class - represents a generic scene element.
Holds a nodes local and world transform.
Class has base update and draw functions which can be overriden by child classes.
*/

#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "OGLRenderer.h"

class SceneNode
{
public:
	SceneNode(Mesh* m = nullptr, Vector4 colour = Vector4(1, 1, 1, 1), Shader* = nullptr, bool cleanupMeshOnDelete = true);
	~SceneNode();

	/* Various utility functions to set and get the nodes state. */
	inline void SetTransform(const Matrix4 &m) { transform = m; }
	inline const Matrix4& GetTransform() const { return transform; }
	inline Matrix4 GetWorldTransform() const { return worldTransform; }

	inline Vector4& GetColour()			{ return colour; }
	inline void SetColour(Vector4 c)	{ colour = c; }

	inline Vector3 GetModelScale() const { return modelScale; }
	inline void SetModelScale(Vector3 s) { modelScale = s; }

	inline Mesh* GetMesh() const { return mesh; }
	inline void SetMesh(Mesh* m) { mesh = m; }
	
	inline SceneNode* GetChild(unsigned int i) const	{ return children[i]; }
	inline unsigned int GetNumberNodes() const			{ return children.size(); }

	inline Shader* getShaderProg() const	{ return nodeShader; }
	inline GLuint getTextureID() const		{ return mesh->GetTexture(); }

	inline float GetBoundingRadius() const { return boundingRadius; }
	inline void SetBoundingRadius(float f) { boundingRadius = f; }

	inline float GetCamDist() const	{ return distCamera; }
	inline void SetCamDist(float f) { distCamera = f; }

	inline void SetShadowCaster(bool b)		{ castShadow = b; }
	inline void SetDirectionalLight(bool b) { directionalLight = b; }
	inline bool IsShadowCaster() const		{ return castShadow; }
	inline bool IsDirectionalLight() const	{ return directionalLight; }

	static bool CompareByCamDist(SceneNode* a, SceneNode* b) { return a->distCamera < b->distCamera; }

	void AddChild(SceneNode* s);

	//Update this nodes local and world transforms.
	virtual void Update(float msec);
	//Draws this nodes mesh if a mesh is present.
	virtual void DrawNode(bool shadowPass = false);

	inline unsigned int getInstanceID() const { return ID; }
	static OGLRenderer* context;
	
protected:
	SceneNode* parent;
	Mesh* mesh;
	Shader* nodeShader;
	vector<SceneNode*> children;

	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;

	float distCamera;
	float boundingRadius;
	//This nodes unique ID.
	unsigned int ID;

	bool deleteMesh;
	bool castShadow = false;
	bool directionalLight = false;

	//A counter essentially which is incremented every time a Scene Node is created.
	static unsigned int current_ID;
};