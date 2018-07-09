#include "stdafx.h"

#include "AnimatedModel.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

glm::vec3 Interpolate(glm::vec3 prev, glm::vec3 next, float interpolant) {
	return glm::mix(prev, next, interpolant);
}

glm::quat Interpolate(glm::quat prev, glm::quat next, float interpolant) {
	return glm::mix(prev, next, interpolant);
}

glm::mat4 CalcPosition(AnimChannel& channel, double tick) {
	glm::mat4 identity = glm::mat4(1.0);
	if (channel.NumPositionKeys() == 0) {
		return identity;
	}
	if (channel.NumPositionKeys() == 1) {
		return glm::translate(identity, channel.GetPositionKey(0).value);
	}

	size_t prev_index = channel.GetPositionKeyIndex(tick);
	VectorKey prev = channel.GetPositionKey(prev_index);
	VectorKey next = prev;
	if (prev_index + 1 < channel.NumPositionKeys()) {
		next = channel.GetPositionKey(prev_index + 1);
	}

	double delta = next.time - prev.time;
	double interpolant = (tick - prev.time) / delta;

	interpolant = glm::clamp(interpolant, 0.0, 1.0);
	glm::vec3 interpolated = Interpolate(prev.value, next.value, static_cast<float>(interpolant));

	return glm::translate(identity, interpolated);
}

glm::mat4 CalcRotation(AnimChannel& channel, double tick) {
	glm::mat4 identity = glm::mat4(1.0);
	if (channel.NumRotationKeys() == 0) {
		return identity;
	}
	if (channel.NumRotationKeys() == 1) {
		return glm::mat4_cast(channel.GetRotationKey(0).value);
	}

	size_t prev_index = channel.GetRotationKeyIndex(tick);
	QuaternionKey prev = channel.GetRotationKey(prev_index);
	QuaternionKey next = prev;
	if (prev_index + 1 < channel.NumRotationKeys()) {
		next = channel.GetRotationKey(prev_index + 1);
	}

	double delta = next.time - prev.time;
	double interpolant = (tick - prev.time) / delta;

	interpolant = glm::clamp(interpolant, 0.0, 1.0);
	glm::quat interpolated = Interpolate(prev.value, next.value, static_cast<float>(interpolant));

	return glm::mat4_cast(interpolated);
}

glm::mat4 CalcScaling(AnimChannel& channel, double tick) {
	glm::mat4 identity = glm::mat4(1.0);
	if (channel.NumScalingKeys() == 0) {
		return identity;
	}
	if (channel.NumScalingKeys() == 1) {
		return glm::scale(identity, channel.GetScalingKey(0).value);
	}

	size_t prev_index = channel.GetScalingKeyIndex(tick);
	VectorKey prev = channel.GetScalingKey(prev_index);
	VectorKey next = prev;
	if (prev_index + 1 < channel.NumScalingKeys()) {
		next = channel.GetScalingKey(prev_index + 1);
	}

	double delta = next.time - prev.time;
	double interpolant = (tick - prev.time) / delta;

	interpolant = glm::clamp(interpolant, 0.0, 1.0);
	glm::vec3 interpolated = Interpolate(prev.value, next.value, static_cast<float>(interpolant));

	return glm::scale(identity, interpolated);
}

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

	double tick = time * animation.GetTickRate();

	tick = std::fmod(tick, animation.GetDuration());

	UpdateTransformsHierarchy(*root, animation, tick, glm::mat4(1.0f));
}

size_t AnimatedModel::AddBone(Bone * bone)
{
	size_t bone_index = bones.size();
	bones.emplace_back(std::move(bone));
	bone_mapping[bone->name] = bone_index;
	return bone_index;
}

Bone & AnimatedModel::GetBone(size_t index)
{
	assert(bones.size() > index);
	return *(bones[index]);
}

void AnimatedModel::UpdateTransformsHierarchy(ModelNode& node, Animation& animation, double tick, glm::mat4 parent_transform)
{
	glm::mat4 node_transform = node.GetTransform();

	if (animation.HasChannel(node.GetName()))
	{
		AnimChannel& channel = animation.GetChannel(node.GetName());

		glm::mat4 translation = CalcPosition(channel, tick);
		glm::mat4 rotation = CalcRotation(channel, tick);
		glm::mat4 scaling = CalcScaling(channel, tick);

		node_transform = translation * rotation * scaling;
	}

	glm::mat4 global_transform = parent_transform * node_transform;

	auto found = bone_mapping.find(node.GetName());
	if (found != bone_mapping.end()) {
		Bone& bone = *(bones[found->second]);
		bone.transform = inverse_root_transform * global_transform * bone.offset;
	}

	node.ForEachChild([&](ModelNode& child) {
		UpdateTransformsHierarchy(child, animation, tick, global_transform);
	});
}
