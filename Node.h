#pragma once

#include "Mesh.h"

#include <memory>
#include <functional>

class Node
{
public:
	Node(std::string name);
	std::string& GetName();
	void AddChild(Node* child);
	std::vector<Node*>& GetChildren();
	void AddMesh(Mesh* mesh);
	void ForEachMesh(const std::function<void(Mesh&)>& f);
	void SetTransform(glm::mat4 transform);
	glm::mat4& GetTransform();
private:
	std::string name;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<Node*> children;
	glm::mat4 transform;
};
