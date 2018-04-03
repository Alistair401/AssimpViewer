#include "stdafx.h"
#include "Mesh.h"

void Mesh::AddVertex(MeshVertex & vertex)
{
	vertices.push_back(vertex);
}

void Mesh::AddIndex(GLuint index)
{
	indices.push_back(index);
}

void Mesh::AddWeight(unsigned int vertex_index, unsigned int bone_index, GLuint bone_id, GLfloat weight)
{
	vertices[vertex_index].bone_ids[bone_index] = bone_id;
	vertices[vertex_index].bone_weights[bone_index] = weight;
}

void Mesh::AddBone(Bone* bone)
{
	bones.push_back(bone);
}

std::vector<Bone*>& Mesh::GetBones()
{
	return bones;
}

void Mesh::GenBuffers()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//GLuint ssbo_index = glGetProgramResourceIndex(shader_program, GL_SHADER_STORAGE_BLOCK, "bone_buffer");
	//glGenBuffers(1, &ssbo);
	//glShaderStorageBlockBinding(shader_program, ssbo_index, ssbo_binding);
}

void Mesh::Render()
{
	//std::vector<glm::mat4> bone_tranforms;
	//for (Bone* bone : bones) {
	//	bone_tranforms.push_back(bone->transform);
	//}

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, bones.size() * sizeof(glm::mat4), &bone_tranforms[0], GL_STATIC_DRAW);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, ssbo);

	GLsizei stride = sizeof(MeshVertex);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0); // pos xyz
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(1); // norm xyz
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2); // color rgb
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 6));

	glEnableVertexAttribArray(3); // tex uv
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 9));

	glEnableVertexAttribArray(4); // bone_ids abcd
	glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, stride, (const void*)(sizeof(float) * 11));

	glEnableVertexAttribArray(8); // bone_weights abcd
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(float) * 11) + sizeof(unsigned int) * 4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
}
