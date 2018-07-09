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
	std::unique_ptr<ModelNode> root;
	Shader* shader = nullptr;
	glm::mat4 inverse_root_transform;
	void SetBuffered(bool buffered);
	bool IsBuffered();
protected:
	bool buffered = false;
};
