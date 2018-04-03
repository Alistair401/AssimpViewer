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
		bone->transform = inverse_root_transform * global_transform * bone->offset;
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
	for (Mesh* mesh : node->GetMeshes()) {
		std::vector<glm::mat4> bone_tranforms;
		for (Bone* bone : mesh->bones) {
			bone_tranforms.push_back(bone->transform);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh->bbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, mesh->NumBones() * sizeof(glm::mat4), &bone_tranforms[0], GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mesh->bbo_binding, mesh->bbo);

		GLsizei stride = sizeof(MeshVertex);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

		glEnableVertexAttribArray(0); // pos xyz
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glEnableVertexAttribArray(1); // norm xyz
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 3));

		glEnableVertexAttribArray(2); // color rgb
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 6));

		glEnableVertexAttribArray(3); // tex uv
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 9));

		glEnableVertexAttribArray(4); // bone_ids abcd
		glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 11));

		glEnableVertexAttribArray(5); // bone_weights abcd
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(GLfloat) * 11) + sizeof(GLuint) * 4));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->NumIndices()), GL_UNSIGNED_INT, 0);
	}

	for (auto child : node->GetChildren())
	{
		RenderHierarchy(child);
	}
}

void Model::GenBufferHierarchy(Node* node) {
	for (Mesh* mesh : node->GetMeshes()) {

		glGenBuffers(1, &mesh->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh->NumVertices() * sizeof(MeshVertex), &mesh->vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &mesh->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->NumIndices() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);

		GLuint ssbo_index = glGetProgramResourceIndex(shader->ID(), GL_SHADER_STORAGE_BLOCK, "bone_buffer");
		glGenBuffers(1, &mesh->bbo);
		glShaderStorageBlockBinding(shader->ID(), ssbo_index, mesh->bbo_binding);
	}

	for (auto child : node->GetChildren())
	{
		GenBufferHierarchy(child);
	}
}

void Model::RegisterBone(Bone * bone)
{
	bones[bone->name] = bone;
}

void Model::Render()
{
	shader->Use();
	if (!buffered) {
		GenBufferHierarchy(root);
		buffered = true;
	}
	RenderHierarchy(root);
}

void Model::SetInverseRootTransform(glm::mat4 transform)
{
	this->inverse_root_transform = transform;
}
