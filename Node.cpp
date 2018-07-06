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
	meshes.emplace_back(std::move(mesh));
}

void Node::ForEachMesh(const std::function<void(Mesh&)>& f)
{
	for (auto&& mesh : meshes) {
		f(*mesh);
	}
}

void Node::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

glm::mat4 & Node::GetTransform()
{
	return transform;
}

void Node::AddChild(Node * child)
{
	children.push_back(child);
}

std::string & Node::GetName()
{
	return name;
}
