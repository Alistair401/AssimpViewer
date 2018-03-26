#pragma once
#include "Node.h"

class Model
{
public:
	void SetRoot(Node* root);
	void Render();
private:
	Node * root = nullptr;
	bool hasMeshes = false;
	bool hasTextures = false;
	bool hasAnimations = false;
};
