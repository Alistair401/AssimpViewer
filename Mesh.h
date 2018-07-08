#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

struct MeshVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 uv;
	glm::uvec4 bone_ids;
	glm::vec4 bone_weights;
};

struct Bone {
	std::string name;
	glm::mat4 offset;
	glm::mat4 transform;
};

class Mesh
{
public:
	void AddVertex(MeshVertex& vertex);
	void AddIndex(GLuint index);
	void AddWeight(unsigned int vertex_index, unsigned int bone_index, GLuint bone_id, GLfloat weight);
	void AddBone(size_t index);
	size_t NumVertices();
	size_t NumBones();
	size_t NumIndices();
	std::vector<MeshVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<size_t> bones;
	GLuint vbo;
	GLuint ibo;
	GLuint bbo;
	GLuint bbo_binding = 1;
};
