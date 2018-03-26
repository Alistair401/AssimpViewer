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
	void RenderHierarchy();
private:
	std::string name;
	std::vector<Mesh*> meshes;
	std::vector<Node*> children;
	void RenderHierarchy(std::string placeholder);
	void Render();
};

