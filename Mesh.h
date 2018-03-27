#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

struct MeshVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 uv;
};

class Mesh
{
public:
	void AddVertex(MeshVertex& vertex);
	void AddIndex(GLuint index);
	void GenBuffers();
	void Render();
private:
	std::vector<MeshVertex> vertices;
	std::vector<GLuint> indices;
	GLuint vbo;
	GLuint ibo;
};
