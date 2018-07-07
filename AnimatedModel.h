#pragma once
#include "Model.h"

class AnimatedModel : public Model
{
public:
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void Update(double delta);
	void RegisterBone(Bone* bone);
private:
	double current_time = 0.0;
	size_t current_animation = 0u;
	std::unordered_map<std::string, size_t> animation_mapping;
	std::vector<std::unique_ptr<Animation>> animations;
	std::unordered_map<std::string, Bone*> bones;
	std::vector<Bone*> bone_arr;

	void UpdateTransformsHierarchy(Node & node, Animation & animation, double tick, glm::mat4 parent_transform);
};

