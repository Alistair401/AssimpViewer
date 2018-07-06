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
	bool IsAnimated();
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void SetShader(Shader* shader);
	Shader& GetShader();
	void Update(double delta);
	void RegisterBone(Bone* bone);
	void Render();
	void SetInverseRootTransform(glm::mat4 transform);
private:
	Shader * shader = nullptr;
	bool buffered = false;
	Node * root = nullptr;
	glm::mat4 inverse_root_transform;

	bool is_animated = false;
	double current_time = 0.0;
	std::string current_animation;
	std::unordered_map<std::string, Animation*> animations;
	std::unordered_map<std::string, Bone*> bones;
	std::vector<Bone*> bone_arr;

	void RenderHierarchy(Node* node);
	void UpdateTransformsHierarchy(Node* node, Animation* animation, double tick, glm::mat4 parent_transform);
	void GenBufferHierarchy(Node* root);
};
