#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

struct MeshVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 uv;
	GLuint bone_ids[4];
	GLfloat bone_weights[4];
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
	void AddBone(Bone* bone);
	std::vector<Bone*>& GetBones();
	void GenBuffers();
	void Render();
private:
	std::vector<MeshVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Bone*> bones;
	GLuint vbo;
	GLuint ibo;
	GLuint ssbo;
	GLuint ssbo_binding =  1;
};
