#include "Mesh.h"
unsigned int Mesh::bytes_used = 0;
Mesh::Mesh()
{
	for (int i = 0; i < MAX_BUFFER; ++i)
	{
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);

	numVertices = 0;
	vertices = nullptr;
	colours = nullptr;
	texture = 0;
	bumpTex = 0;
	textureCoords = nullptr;
	normals = nullptr;
	tangents = nullptr;
	type = GL_TRIANGLES;
	patchVerts = 0;
	scale = 0.0f;
	rotation = 0.0f;
	position = Vector3(0, 0, 0);
	indices = nullptr;
	numIndices = 0;
	modelMatrix = Matrix4::Translation(position);
	specularFactor = 0.0f;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &shadow_tex);
	delete[] vertices;
	delete[] colours;
	delete[] textureCoords;
	delete[] indices;
	delete[] normals;
	delete[] tangents;
}

void Mesh::GenerateNormals()
{
	if (!normals)
	{
		normals = new Vector3[numVertices];
	}
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		normals[i] = Vector3();
	}
	if (indices)
	{
		for (unsigned int i = 0; i < numIndices; i += 3)
		{
			unsigned int a = indices[i];
			unsigned int b = indices[i + 1];
			unsigned int c = indices[i + 2];

			Vector3 normal = Vector3::Cross(vertices[b] - vertices[a], vertices[c] - vertices[a]);
			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	}
	else
	{
		for (unsigned int i = 0; i < numVertices; i += 3)
		{
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i + 1];
			Vector3 &c = vertices[i + 2];
			
			Vector3 normal = Vector3::Cross(c - a, b - a);
			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents()
{
	if (!tangents) 
	{
		tangents = new Vector3[numVertices];
	}
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		tangents[i] = Vector3();
	}
	if (indices) 
	{
		for (unsigned int i = 0; i < numIndices; i += 3)
		{
			unsigned int a = indices[i];
			unsigned int b = indices[i + 1];
			unsigned int c = indices[i + 2];
			
			Vector3 tangent = GenerateTangent(vertices[a], vertices[b],
				vertices[c], textureCoords[a], textureCoords[b], textureCoords[c]);
			
			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else 
	{
		for (unsigned int i = 0; i < numVertices; i += 3)
		{
			Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1],
				vertices[i + 2], textureCoords[i],
				textureCoords[i + 1], textureCoords[i + 2]);
			
			tangents[i] += tangent;
			tangents[i + 1] += tangent;
			tangents[i + 2] += tangent;
		}
	}
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
	const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc)
{
	Vector2 coord1 = tb - ta;
	Vector2 coord2 = tc - ta;
	
	Vector3 vertex1 = b - a;
	Vector3 vertex2 = c - a;
	
	Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);
	
	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);
	return axis * factor;
}

Mesh* Mesh::GenerateTriangle()
{
	Mesh* m = new Mesh();
	m->numVertices = 3;
	m->patchVerts = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f, 0.5f, -2.0f);
	m->vertices[1] = Vector3(0.5f, -0.5f, -2.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f, -2.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateQuad(bool zQuad)
{
	Mesh* m = new Mesh();
	m->numVertices = 4;
	m->numIndices = 4;
	m->patchVerts = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];
	
	if (zQuad)
	{
		//z axis quad
		m->vertices[0] = Vector3(-1.0f, 0.0f, -1.0f);
		m->vertices[1] = Vector3(-1.0f, 0.0f, 1.0f);
		m->vertices[2] = Vector3(1.0f, 0.0f, -1.0f);
		m->vertices[3] = Vector3(1.0f, 0.0f, 1.0f);
	}
	else
	{
		//y axis quad
		m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
		m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
		m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
		m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);
	}
	
	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (unsigned int i = 0; i < m->numVertices; ++i)
	{
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		if (zQuad)
		{
			m->normals[i] = Vector3(0.0f, 1.0f, 0.0f);
			m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
			m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
		}
	}
	m->BufferData();
	return m;
}

void Mesh::BufferData() 
{
	glBindVertexArray(arrayObject);
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof (Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);
	Mesh::bytes_used += sizeof(Vector3)* numVertices;

	if (textureCoords)
	{
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
		Mesh::bytes_used += sizeof(Vector2)* numVertices;
	}
	if (colours) 
	{ // Just in case the data has no colour attribute ...
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof (Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
		Mesh::bytes_used += sizeof(Vector4)* numVertices;
	}
	if (indices)
	{
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
		Mesh::bytes_used += sizeof(GLuint)* numVertices;
	}
	if (normals)
	{
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
		Mesh::bytes_used += sizeof(Vector3)* numVertices;
	}
	if (tangents)
	{
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof (Vector3), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
		Mesh::bytes_used += sizeof(Vector3)* numVertices;
	}
	glBindVertexArray(0);
}

void Mesh::SetBumpMap(GLuint target)
{
	int w, h;
	bumpTex = target;
	glBindTexture(GL_TEXTURE_2D, target);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

	Mesh::bytes_used += sizeof(float)* (w * h);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::SetTexture(GLuint tex)
{
	int w, h;
	texture = tex;
	glBindTexture(GL_TEXTURE_2D, tex);
	glGenerateMipmap(GL_TEXTURE_2D);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

	Mesh::bytes_used += sizeof(float) * (w * h);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTex);

	glBindVertexArray(arrayObject);

	if (patchVerts != 0)
	{
		glPatchParameteri(GL_PATCH_VERTICES, patchVerts);
	}

	if (indices)
	{
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(type, 0, numVertices);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}