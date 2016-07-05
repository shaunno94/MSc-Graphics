#include "SceneNode.h"

unsigned int SceneNode::current_ID = 0;
OGLRenderer* SceneNode::context = nullptr;

SceneNode::SceneNode(Mesh* mesh, Vector4 colour, bool cleanupMeshOnDelete)
{
	this->mesh = mesh;
	this->colour = colour;
	this->deleteMesh = cleanupMeshOnDelete;
	parent = nullptr;
	nodeShader = nullptr;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distCamera = 0.0f;
	ID = SceneNode::current_ID++;
}

SceneNode::~SceneNode()
{	
	if (mesh && deleteMesh)
	{
		delete mesh;
		mesh = nullptr;
	}
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		if (children[i])
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
	if (nodeShader)
	{
		//delete nodeShader;
		nodeShader = nullptr;
	}
}

void SceneNode::AddChild(SceneNode* s)
{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::DrawNode(bool shadowPass)
{
	if (mesh)
	{
		mesh->Draw();
	}
}

void SceneNode::Update(float msec)
{
	if (parent)
	{
		worldTransform = parent->worldTransform * transform;
	}
	else
	{
		worldTransform = transform;
	}
	if (mesh)
	{
		mesh->setModelMatrix(worldTransform * Matrix4::Scale(modelScale));
	}
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		children[i]->Update(msec);
	}
}