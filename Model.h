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
	void SetRoot(ModelNode* root);
	ModelNode& GetRoot();
	void SetShader(Shader* shader);
	Shader& GetShader();
	void SetInverseRootTransform(glm::mat4 transform);
	void SetBuffered(bool buffered);
	bool IsBuffered();
protected:
	Shader * shader = nullptr;
	bool buffered = false;
	std::unique_ptr<ModelNode> root;
	glm::mat4 inverse_root_transform;
};
