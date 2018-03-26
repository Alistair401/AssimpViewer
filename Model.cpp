#include "stdafx.h"
#include "Model.h"

void Model::SetRoot(Node * root)
{
	this->root = root;
}

void Model::Render()
{
	root->RenderHierarchy();
}
