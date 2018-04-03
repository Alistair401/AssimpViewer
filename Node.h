#pragma once
#include <vector>
#include "Mesh.h"

class Node
{
public:
	Node(std::string name);
	std::string& GetName();
	void AddChild(Node* child);
	std::vector<Node*>& GetChildren();
	void AddMesh(Mesh* mesh);
	std::vector<Mesh*>& GetMeshes();
	void SetTransform(glm::mat4 transform);
	glm::mat4& GetTransform();
private:
	std::string name;
	std::vector<Mesh*> meshes;
	std::vector<Node*> children;
	glm::mat4 transform;
};
