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
	void SetRoot(Node* root);
	Node& GetRoot();
	void SetShader(Shader* shader);
	Shader& GetShader();
	void SetInverseRootTransform(glm::mat4 transform);
	void SetBuffered(bool buffered);
	bool IsBuffered();
protected:
	Shader * shader = nullptr;
	bool buffered = false;
	std::unique_ptr<Node> root;
	glm::mat4 inverse_root_transform;
};
