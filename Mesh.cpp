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

void Mesh::GenBuffers()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Mesh::Render()
{
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
}
