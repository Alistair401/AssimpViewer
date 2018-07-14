#pragma once

#include "Shader.h"
#include "Node.h"
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
	Node* root;
	void RegisterNode(Node* node);

	Shader* shader = nullptr;
	glm::mat4 inverse_root_transform;
	void SetBuffered(bool buffered);
	bool IsBuffered();
protected:
	std::unordered_map<std::string, Node*> node_mapping;
	bool buffered = false;
};
