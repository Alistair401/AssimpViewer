#pragma once

#include "Mesh.h"

#include <memory>
#include <functional>

class Node
{
public:
	Node(std::string name);
	glm::mat4 transform;
	std::string name;
	std::shared_ptr<Bone> bone = nullptr;

	void AddChild(Node* child);
	void ForEachChild(const std::function<void(Node&)>& f);
	void AddMesh(Mesh* mesh);
	void ForEachMesh(const std::function<void(Mesh&)>& f);
private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<Node*> children;
};
