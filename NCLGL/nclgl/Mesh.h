/*
Author: Rich Davison
Modified by: Shaun Heald
Base mesh class, repsonsible for buffering vertex (creating VAO, VBO's etc)
data such as vertices, indices, normals, tangents.
Class has functions to generate normals and tangents based on suplied vertex data.
Class can generate simple shapes such as a triangle / quad.
For more complex shapes or loading data from a file this class should be extended.
Class has a simple draw function that can be overriden. 
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
	
	//Functions to generate simple shapes.
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad(bool zQuad = false);
	//How mnay bytes are meshes using
	static unsigned int GetBytesUsed() { return Mesh::bytes_used; }
	
	/* Getter/setter functions. */
	void SetTexture(GLuint tex);
	inline GLuint GetTexture(){ return texture; }

	void SetBumpMap(GLuint tex);
	inline GLuint GetBumpMap() const { return bumpTex; }
	
	inline void setPos(Vector3& pos){ position = pos; }
	inline Vector3 getPos() const { return position; }
	
	inline void setRot(float rot){ rotation = rot; }
	inline float getRot() const { return rotation; }
	
	inline void setScale(float scale){ this->scale = scale; }
	inline float getScale() const { return scale; }

	inline Matrix4& getModelMatrix(){ return modelMatrix; }
	inline void setModelMatrix(Matrix4& m){ modelMatrix = m; }

	inline float GetSpecularFactor() const { return specularFactor; }

	inline void setShadowTex(GLuint target) { shadow_tex = target; }
	inline GLuint getShadowTex() { return shadow_tex; }

	//Update primitve type.
	inline void updateType(GLuint t) { type = t; }

protected:
	//Create OpenGL VAO and various VBO's.
	void BufferData();
	//Generate normals based on vertices. normalise(cross(b - a, c - a))
	void GenerateNormals();
	//Generate tangents based on vertices / texture co-ordinates.
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc);

	//VAO handle
	GLuint arrayObject;
	//VBO handles
	GLuint bufferObject[MAX_BUFFER];
	
	GLuint numVertices;
	GLuint numIndices;
	
	//Primitve type
	GLuint type;
	//How many vertices per patch
	GLuint patchVerts;
	
	//Texture handles
	GLuint texture;
	GLuint bumpTex;	
	GLuint shadow_tex;
	
	//Various vertex data
	unsigned int* indices;
	Vector3* vertices;
	Vector4* colours;
	Vector2* textureCoords;
	Vector3* normals;
	Vector3* tangents;

	//Local transform
	float scale;
	float rotation;
	Vector3 position;
	Matrix4 modelMatrix;

	float specularFactor;

	static unsigned int bytes_used;
};