/*
Class:OBJMesh
Implements:Mesh
Author:Rich Davison
Description: Implementation of the Wavefront OBJ mesh format. 
*/

#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "Vector3.h"
#include "Vector2.h"
#include "Mesh.h"
#include "ChildMeshInterface.h"

#define OBJOBJECT	"object"	//the current line of the obj file defines the start of a new material
#define OBJUSEMTL	"usemtl"	//the current line of the obj file defines the start of a new material
#define OBJMESH		"g"			//the current line of the obj file defines the start of a new face
#define OBJCOMMENT	"#"			//The current line of the obj file is a comment
#define OBJVERT		"v"			//the current line of the obj file defines a vertex
#define OBJTEX		"vt"		//the current line of the obj file defines texture coordinates
#define OBJNORM		"vn"		//the current line of the obj file defines a normal
#define OBJFACE		"f"			//the current line of the obj file defines a face
#define OBJMTLLIB "mtllib"		//the current line of the obj file define a material
#define OBJSHADING "s"			//the current line of the obj file defines polygon shading

/*
OBJSubMesh structs are used to temporarily keep the data loaded 
in from the OBJ files, before being parsed into a series of
Meshes
*/
struct OBJSubMesh {
	std::vector<int> texIndices;
	std::vector<int> vertIndices;
	std::vector<int> normIndices;

	int indexOffset;
};

class OBJMesh : public Mesh, public ChildMeshInterface	{
public:
	OBJMesh(void){};
	OBJMesh(std::string filename){LoadOBJMesh(filename);};
	~OBJMesh(void){};
	bool	LoadOBJMesh(std::string filename);

	virtual void Draw();
};

