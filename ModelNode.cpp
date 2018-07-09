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
