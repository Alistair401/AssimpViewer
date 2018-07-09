#pragma once

#include "Mesh.h"

#include <memory>
#include <functional>

class ModelNode
{
public:
	ModelNode(std::string name);
	std::string& GetName();
	void AddChild(ModelNode* child);
	void ForEachChild(const std::function<void(ModelNode&)>& f);
	void AddMesh(Mesh* mesh);
	void ForEachMesh(const std::function<void(Mesh&)>& f);
	void SetTransform(glm::mat4 transform);
	glm::mat4& GetTransform();
private:
	std::string name;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<ModelNode>> children;
	glm::mat4 transform;
};
