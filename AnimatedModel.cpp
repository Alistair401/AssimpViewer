#include "stdafx.h"

#include "AnimatedModel.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

void AnimatedModel::AddAnimation(Animation * animation)
{
	animation_mapping[animation->GetName()] = animations.size();
	animations.emplace_back(std::move(animation));
}

void AnimatedModel::SetAnimation(std::string name)
{
	auto found = animation_mapping.find(name);

	assert(found != animation_mapping.end());

	current_animation = found->second;
}

void AnimatedModel::Update(double delta)
{
	Animation& animation = *(animations[current_animation]);

	time += delta;

	std::unordered_map<std::string, glm::mat4> pose = AnimationEvaluator::Evaluate(animation, time);

	UpdateTransformsHierarchy(*root, pose, glm::mat4(1.0f));
}

void AnimatedModel::AddBone(std::shared_ptr<Bone> bone)
{
	node_mapping[bone->name]->bone = bone;

}

void AnimatedModel::UpdateTransformsHierarchy(Node& node, Pose& pose, glm::mat4 parent_transform)
{
	auto node_found = pose.find(node.name);
	if (node_found != pose.end())
	{
		node.transform = node_found->second;
	}

	glm::mat4 global_transform = parent_transform * node.transform;

	if (node.bone != nullptr) {
		node.bone->transform = inverse_root_transform * global_transform * node.bone->offset;
	}

	node.ForEachChild([&](Node& child) {
		UpdateTransformsHierarchy(child, pose, global_transform);
	});
}
