#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>


Shader::Shader()
{
}

void Shader::AddComponent(std::string filename, GLenum type)
{
	std::ifstream file_stream;

	try {
		file_stream.open(filename);
	}
	catch (std::ifstream::failure e) {
		std::cout << "Failed to load file: " << filename << std::endl;
		return;
	}

	std::stringstream content_buffer;
	content_buffer << file_stream.rdbuf();
	file_stream.close();
	std::string source = content_buffer.str().c_str();

	const char* source_chars = source.c_str();

	GLuint component = glCreateShader(type);
	glShaderSource(component, 1, &source_chars, NULL);
	glCompileShader(component);

	GLint success;
	char log[512];
	glGetShaderiv(component, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(component, 512, NULL, log);
		std::cout << "Failed to compile shader component: " << filename << std::endl;
		std::cout << log << std::endl;
		return;
	}

	components.push_back(component);
}

void Shader::Link()
{
	program = glCreateProgram();
	for (auto component : components)
	{
		glAttachShader(program, component);
	}
	glLinkProgram(program);

	GLint success;
	char log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "Failed to link shader program" << std::endl;
		std::cout << log << std::endl;
		return;
	}
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::setMat4(const std::string & name, const glm::mat4 & mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
