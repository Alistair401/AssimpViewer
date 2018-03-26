#include "stdafx.h"
#include "Node.h"

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
}

void Node::AddChild(Node * child)
{
	children.push_back(child);
}

std::string & Node::GetName()
{
	return name;
}
