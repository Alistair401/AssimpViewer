#include "stdafx.h"
#include "Node.h"
#include <GL\glew.h>

Node::Node(std::string name)
{
	this->name = name;
}

std::vector<Node*>& Node::GetChildren()
{
	return children;
}

void Node::AddMesh(Mesh * mesh)
{
	meshes.push_back(mesh);
}

std::vector<Mesh*>& Node::GetMeshes()
{
	return meshes;
}

void Node::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

glm::mat4 & Node::GetTransform()
{
	return transform;
}

void Node::Render()
{
	for (auto mesh: meshes)
	{
		mesh->Render();
	}
}

void Node::AddChild(Node * child)
{
	children.push_back(child);
}

std::string & Node::GetName()
{
	return name;
}
