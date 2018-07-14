#pragma once

#include "Shader.h"
#include "ModelNode.h"
#include "Animation.h"

#include <unordered_map>
#include <memory>

struct Key {
	size_t position;
	size_t rotation;
	size_t scaling;
};

class Model
{
public:
	ModelNode* root;
	void RegisterNode(ModelNode* node);

	Shader* shader = nullptr;
	glm::mat4 inverse_root_transform;
	void SetBuffered(bool buffered);
	bool IsBuffered();
protected:
	std::unordered_map<std::string, ModelNode*> node_mapping;
	bool buffered = false;
};
