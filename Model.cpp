#include "stdafx.h"

#include "Model.h"

void Model::SetRoot(Node * root)
{
	this->root = std::unique_ptr<Node>(std::move(root));
}

Node & Model::GetRoot()
{
	return *root;
}

void Model::SetShader(Shader * shader)
{
	this->shader = shader;
}

Shader & Model::GetShader()
{
	assert(shader != nullptr);
	return *shader;
}

void Model::SetInverseRootTransform(glm::mat4 transform)
{
	this->inverse_root_transform = transform;
}

void Model::SetBuffered(bool buffered)
{
	this->buffered = buffered;
}

bool Model::IsBuffered()
{
	return buffered;
}
