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

void Node::RenderHierarchy()
{
	this->RenderHierarchy("");
}

void Node::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

void Node::RenderHierarchy(std::string placeholder)
{
	this->Render();
	for (auto child: children)
	{
		child->RenderHierarchy("");
	}
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
