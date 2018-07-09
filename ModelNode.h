#pragma once

#include "Mesh.h"

#include <memory>
#include <functional>

class ModelNode
{
public:
	ModelNode(std::string name);
	glm::mat4 transform;
	std::string name;

	void AddChild(ModelNode* child);
	void ForEachChild(const std::function<void(ModelNode&)>& f);
	void AddMesh(Mesh* mesh);
	void ForEachMesh(const std::function<void(Mesh&)>& f);
private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<ModelNode>> children;
};
