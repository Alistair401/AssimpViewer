#pragma once

#include "Model.h"
#include "AnimationEvaluator.h"

class AnimatedModel : public Model
{
public:
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void Update(double delta);
	void AddBone(Bone * bone);

private:
	double time = 0.0;
	size_t current_animation = 0u;
	std::unordered_map<std::string, size_t> animation_mapping;
	std::vector<std::unique_ptr<Animation>> animations;

	void UpdateTransformsHierarchy(ModelNode & node, Pose& pose, glm::mat4 parent_transform);
};
