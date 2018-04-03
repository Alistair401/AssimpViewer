#pragma once
#include "Node.h"
#include "Animation.h"
#include <unordered_map>
#include "Shader.h"

class Model
{
public:
	void SetRoot(Node* root);
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void SetShader(Shader* shader);
	void Update(double delta);
	void RegisterBone(Bone* bone);
	void Render();
	void SetInverseRootTransform(glm::mat4 transform);
private:
	bool buffered = false;
	Shader* shader = nullptr;
	Node * root = nullptr;
	double current_time = 0.0;
	std::string current_animation;
	std::unordered_map<std::string, Animation*> animations;
	std::unordered_map<std::string, Bone*> bones;
	glm::mat4 inverse_root_transform;
	void UpdateTransformsHierarchy(Node* node, Animation* animation, double tick, glm::mat4 parent_transform);
	void GenBufferHierarchy(Node* root);
};
