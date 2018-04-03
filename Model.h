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
private:
	Shader* shader = nullptr;
	Node * root = nullptr;
	double current_time = 0.0;
	std::string current_animation;
	std::unordered_map<std::string, Animation*> animations;
	std::unordered_map<std::string, Bone*> bones;
	void UpdateTransformsHierarchy(Node* node, Animation* animation, double tick, glm::mat4 parent_transform);
};
