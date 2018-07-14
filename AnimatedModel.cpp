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

size_t AnimatedModel::AddBone(Bone * bone)
{
	size_t bone_index = bones.size();
	bones.push_back(bone);
	bone_mapping[bone->name] = bone_index;
	return bone_index;
}

Bone & AnimatedModel::GetBone(size_t index)
{
	assert(bones.size() > index);
	return *(bones[index]);
}

void AnimatedModel::UpdateTransformsHierarchy(ModelNode& node, Pose& pose, glm::mat4 parent_transform)
{
	auto node_found = pose.find(node.name);
	if (node_found != pose.end())
	{
		node.transform = node_found->second;
	}

	glm::mat4 global_transform = parent_transform * node.transform;

	auto bone_found = bone_mapping.find(node.name);
	if (bone_found != bone_mapping.end()) {
		Bone& bone = *(bones[bone_found->second]);
		bone.transform = inverse_root_transform * global_transform * bone.offset;
	}

	node.ForEachChild([&](ModelNode& child) {
		UpdateTransformsHierarchy(child, pose, global_transform);
	});
}
