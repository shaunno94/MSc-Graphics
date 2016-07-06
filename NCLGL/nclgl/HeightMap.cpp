#include "HeightMap.h"

HeightMap::HeightMap(const char* file, const char* heights_nrm, float scale, const char* baseTex, const char* bumpMapTex)
{
	ifstream inFile(file, ios::binary);
	if (!inFile)
	{
		cout << "Heightmap data file not found." << endl;
		system("pause");
		exit(1);
	}
	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH * RAW_HEIGHT) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];
	heightmap_scale = scale;
	for (int x = 0; x < RAW_WIDTH; ++x) 
	{
		for (int z = 0; z < RAW_HEIGHT; ++z) 
		{
			int offset = (x * RAW_WIDTH) + z;
			textureCoords[offset] = Vector2(float(x) / float(RAW_WIDTH), float(z) / float(RAW_HEIGHT));
			vertices[offset] = Vector3(x * heightmap_scale, 0, z * heightmap_scale);
		}
	}

	numIndices = 0;
	for (int x = 0; x < RAW_WIDTH - 1; ++x) 
	{
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) 
		{
			indices[numIndices++] = ((x + 1) * RAW_WIDTH) + (z + 1); 
			indices[numIndices++] = ((x + 1) * RAW_WIDTH) + z;
			indices[numIndices++] = (x * RAW_WIDTH) + z;

			indices[numIndices++] = (x * (RAW_WIDTH)) + z;
			indices[numIndices++] = (x * RAW_WIDTH) + (z + 1);
			indices[numIndices++] = ((x + 1) * RAW_WIDTH) + (z + 1);
		}
	}

	height_data = SOIL_load_OGL_texture(file, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::SetTextureRepeating(height_data, false);
	
	SetTexture(SOIL_load_OGL_texture(baseTex, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	OGLRenderer::SetTextureRepeating(texture, true);
	OGLRenderer::SetTextureFiltering(texture, true, true);

	bumpTex = SOIL_load_OGL_texture(bumpMapTex, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::SetTextureRepeating(bumpTex, true);

	heightsNRM = SOIL_load_OGL_texture(heights_nrm, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::SetTextureRepeating(heightsNRM, false);
	
	snowTex = SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "snow.jpg").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::GenerateMipMaps(snowTex);
	OGLRenderer::SetTextureFiltering(snowTex, true, true);

	grassTex = SOIL_load_OGL_texture((File_Locs::TEXTURE_DIR + "grass.jpg").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	OGLRenderer::GenerateMipMaps(grassTex);
	OGLRenderer::SetTextureFiltering(grassTex, true, true);

	modelMatrix = Matrix4::Translation(Vector3(0, 0, 0));
	shine_factor = 15.0f;
	type = GL_PATCHES;
	patchVerts = 3;
	//GenerateNormals();
	//GenerateTangents();
	BufferData();
}

void HeightMap::Draw()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, height_data);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, snowTex);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, heightsNRM);
	Mesh::Draw();
}

HeightMap::~HeightMap(){}