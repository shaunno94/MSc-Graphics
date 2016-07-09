/*
Author: Shaun Heald

*/

#pragma once
#include"OGLRenderer.h"

enum MeshBuffer 
{
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	virtual void Draw();
	
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad(bool zQuad = false);
	static unsigned int GetBytesUsed() { return Mesh::bytes_used; }
	
	void SetTexture(GLuint tex);
	inline GLuint GetTexture(){ return texture; }

	void SetBumpMap(GLuint tex);
	inline GLuint getBumpMap() const { return bumpTex; }
	
	inline void setPos(Vector3& pos){ position = pos; }
	inline Vector3 getPos() const { return position; }
	
	inline void setRot(float rot){ rotation = rot; }
	inline float getRot() const { return rotation; }
	
	inline void setScale(float scale){ this->scale = scale; }
	inline float getScale() const { return scale; }

	inline Matrix4& getModelMatrix(){ return modelMatrix; }
	inline void setModelMatrix(Matrix4& m){ modelMatrix = m; }

	inline float getShininess() const { return shine_factor; }

	inline void setShadowTex(GLuint target) { shadow_tex = target; }
	inline GLuint getShadowTex() { return shadow_tex; }

	inline void updateType(GLuint t) { type = t; }

protected:
	void BufferData();
	void GenerateNormals();
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc);

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	GLuint patchVerts;
	GLuint texture;
	GLuint bumpTex;
	
	GLuint numIndices;
	unsigned int* indices;

	Vector3* vertices;
	Vector4* colours;
	Vector2* textureCoords;
	Vector3* normals;
	Vector3* tangents;

	float scale;
	float rotation;
	Vector3 position;
	Matrix4 modelMatrix;

	float shine_factor;
	GLuint shadow_tex;

	static unsigned int bytes_used;
};