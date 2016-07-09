/*
Class: ChildMeshInterface
Author:Rich Davison 
*/

#pragma once

#include "Mesh.h"
#include <vector>

class ChildMeshInterface	
{
public:
	//Adds a child mesh to this mesh
	void ChildMeshInterface::AddChild(Mesh*m)	
	{
		children.push_back(m);
	}

	//Returns a mesh object from the child vector if i < size of children.
	inline Mesh* ChildMeshInterface::GetChild(unsigned int i) { return i < children.size() ? children[i] : nullptr; }

	virtual ~ChildMeshInterface() 
	{
		for(unsigned int i = 0; i < children.size(); ++i) 
		{
			delete children[i];
		}
	}

protected:
	//Some Meshes have children...
	std::vector<Mesh*>children;
	ChildMeshInterface(void){};
};