#include "stdafx.h"
#include "Model.h"

void Model::SetRoot(Node * root)
{
	this->root = root;
}

void Model::AddAnimation(Animation * animation)
{
	animations[animation->GetName()] = animation;
}

void Model::SetAnimation(std::string name)
{
	current_animation = name;
}

void Model::SetShader(Shader * shader)
{
	this->shader = shader;
}

void Model::UpdateTransformsHierarchy(Node* node, Animation* animation, double tick, glm::mat4 parent_transform) {

	AnimChannel* channel = animation->GetChannel(node->GetName());

	glm::mat4 node_transform = node->GetTransform();

	if (channel != nullptr) {
		// Interpolate and get tranformations
	}

	glm::mat4 global_transform = parent_transform * node_transform;

	if (bones.find(node->GetName()) != bones.end()) {
		Bone* bone = bones[node->GetName()];
		bone->transform = global_transform * bone->offset;
	}

	for (Node* child : node->GetChildren()) {
		UpdateTransformsHierarchy(child, animation, tick, global_transform);
	}
}

void Model::Update(double delta)
{
	if (animations.find(current_animation) == animations.end()) {
		return;
	}

	Animation* animation = animations[current_animation];

	current_time += delta;
	double current_tick = current_time * animation->GetTickRate();

	UpdateTransformsHierarchy(root, animation, current_tick, glm::mat4(1.0f));
}

void RenderHierarchy(Node* node) {
	node->Render();
	for (auto child : node->GetChildren())
	{
		RenderHierarchy(child);
	}
}

void Model::RegisterBone(Bone * bone)
{
	bones[bone->name] = bone;
}

void Model::Render()
{
	shader->Use();
	RenderHierarchy(root);
}
