#pragma once
#include "Model.h"

class AnimatedModel : public Model
{
public:
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void Update(double delta);
	size_t AddBone(Bone* bone);
	Bone& GetBone(size_t index);
private:
	double time = 0.0;
	size_t current_animation = 0u;
	std::unordered_map<std::string, size_t> animation_mapping;
	std::vector<std::unique_ptr<Animation>> animations;

	std::unordered_map<std::string, size_t> bone_mapping;
	std::vector<std::unique_ptr<Bone>> bones;

	void UpdateTransformsHierarchy(Node & node, Animation & animation, double tick, glm::mat4 parent_transform);
};

