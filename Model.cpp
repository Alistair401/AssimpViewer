#include "stdafx.h"

#include "Model.h"

void Model::SetBuffered(bool buffered)
{
	this->buffered = buffered;
}

bool Model::IsBuffered()
{
	return buffered;
}
