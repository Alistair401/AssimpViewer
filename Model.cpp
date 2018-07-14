#include "stdafx.h"

#include "Model.h"

void Model::RegisterNode(Node * node)
{
	node_mapping[node->name] = node;
}

void Model::SetBuffered(bool buffered)
{
	this->buffered = buffered;
}

bool Model::IsBuffered()
{
	return buffered;
}
