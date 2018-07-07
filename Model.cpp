#include "stdafx.h"
#include "Model.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"


void Model::SetRoot(Node * root)
{
	this->root = std::unique_ptr<Node>(std::move(root));
}

Node & Model::GetRoot()
{
	return *root;
}

bool Model::IsAnimated()
{
	return is_animated;
}

void Model::AddAnimation(Animation * animation)
{
	is_animated = true;
	animation_mapping[animation->GetName()] = animations.size();
	animations.emplace_back(std::move(animation));
}

void Model::SetAnimation(std::string name)
{
	auto found = animation_mapping.find(name);

	assert(found != animation_mapping.end());

	current_animation = found->second;
}

void Model::SetShader(Shader * shader)
{
	this->shader = shader;
}

Shader & Model::GetShader()
{
	assert(shader != nullptr);
	return *shader;
}

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

void Model::UpdateTransformsHierarchy(Node& node, Animation& animation, double tick, glm::mat4 parent_transform)
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

	if (bones.find(node.GetName()) != bones.end()) {
		Bone* bone = bones[node.GetName()];
		bone->transform = inverse_root_transform * global_transform * bone->offset;
	}

	node.ForEachChild([&](Node& child) {
		UpdateTransformsHierarchy(child, animation, tick, global_transform);
	});
}

void Model::Update(double delta)
{
	if (!is_animated) {
		return;
	}

	Animation& animation = *(animations[current_animation]);

	current_time += delta;
	double current_tick = current_time * animation.GetTickRate();
	if (current_tick > animation.GetDuration()) {
		current_time = 0;
		current_tick = 0;
	}

	UpdateTransformsHierarchy(*root, animation, current_tick, glm::mat4(1.0f));
}

void Model::RegisterBone(Bone * bone)
{
	bones[bone->name] = bone;
}

void Model::SetInverseRootTransform(glm::mat4 transform)
{
	this->inverse_root_transform = transform;
}

void Model::SetBuffered(bool buffered)
{
	this->buffered = buffered;
}

bool Model::IsBuffered()
{
	return buffered;
}
