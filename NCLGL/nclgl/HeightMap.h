/*
Author: Shaun Heald

*/

#pragma once
#include <iostream>
#include <fstream>
#include "Mesh.h"

class HeightMap : public Mesh
{
public:
	HeightMap(const char* file, const char* heights_nrm, float scale, const char* baseTex, const char* bumpMapTex = "");
	virtual ~HeightMap();

	virtual void Draw();

	inline GLuint getHeightTexData() { return height_data; }
	inline Vector3 getCenter(){ return Vector3(((RAW_HEIGHT * heightmap_scale) / 2.0f), 0, ((RAW_WIDTH * heightmap_scale) / 2.0f)); }

protected:
	const unsigned short RAW_WIDTH = 512;
	const unsigned short RAW_HEIGHT = 512;
	const float HEIGHTMAP_X = 8.0f;
	const float HEIGHTMAP_Z = 8.0f;
	const float HEIGHTMAP_Y = 1.25f;
	const float HEIGHTMAP_TEX_X = 1.0f / 2.0f;
	const float HEIGHTMAP_TEX_Z = 1.0f / 2.0f;
	float heightmap_scale;

	GLuint snowTex;
	GLuint grassTex;
	GLuint height_data;
	GLuint heightsNRM;
};