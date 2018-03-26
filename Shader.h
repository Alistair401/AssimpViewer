#pragma once
#include <vector>
#include <GL\glew.h>

class Shader
{
public:
	Shader();
	void AddComponent(std::string filename, GLenum type);
	void Link();
	void Use();
private:
	std::vector<GLuint> components;
	GLuint program;
};

