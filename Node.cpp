#include "stdafx.h"
#include "ModelNode.h"
#include <GL\glew.h>

ModelNode::ModelNode(std::string name)
{
	this->name = name;
}

void ModelNode::AddMesh(Mesh * mesh)
{
	meshes.emplace_back(std::move(mesh));
}

void ModelNode::ForEachMesh(const std::function<void(Mesh&)>& f)
{
	for (auto&& mesh : meshes) {
		f(*mesh);
	}
}

void ModelNode::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

glm::mat4 & ModelNode::GetTransform()
{
	return transform;
}

void ModelNode::AddChild(ModelNode * child)
{
	children.emplace_back(std::move(child));
}

void ModelNode::ForEachChild(const std::function<void(ModelNode&)>& f)
{
	for (auto&& child : children) {
		f(*child);
	}
}

std::string & ModelNode::GetName()
{
	return name;
}
