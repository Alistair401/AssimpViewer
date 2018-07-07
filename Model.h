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
	bool IsAnimated();
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void SetShader(Shader* shader);
	Shader& GetShader();
	void Update(double delta);
	void RegisterBone(Bone* bone);
	void SetInverseRootTransform(glm::mat4 transform);
	void SetBuffered(bool buffered);
	bool IsBuffered();
private:
	Shader * shader = nullptr;
	bool buffered = false;
	std::unique_ptr<Node> root;
	glm::mat4 inverse_root_transform;

	bool is_animated = false;
	double current_time = 0.0;
	size_t current_animation = 0u;
	std::unordered_map<std::string, size_t> animation_mapping;
	std::vector<std::unique_ptr<Animation>> animations;
	std::unordered_map<std::string, Bone*> bones;
	std::vector<Bone*> bone_arr;

	void UpdateTransformsHierarchy(Node & node, Animation & animation, double tick, glm::mat4 parent_transform);
};
