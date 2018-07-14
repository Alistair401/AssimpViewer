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

size_t Mesh::NumVertices()
{
	return vertices.size();
}

size_t Mesh::NumBones()
{
	return bones.size();
}

size_t Mesh::NumIndices()
{
	return indices.size();
}
