#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>

class Shader
{
public:
	Shader();
	void AddComponent(std::string filename, GLenum type);
	void Link();
	void Use();
	void setMat4(const std::string & name, const glm::mat4 & mat) const;
private:
	std::vector<GLuint> components;
	GLuint program;
};

