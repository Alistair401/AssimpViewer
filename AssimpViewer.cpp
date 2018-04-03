#include "stdafx.h"
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Import.h"
#include "Model.h"
#include "Shader.h"

glm::vec3 camera_vel;

int window_width = 640, window_height = 480;

void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			camera_vel.y = -1;
		}
		if (action == GLFW_RELEASE) {
			camera_vel.y = 0;
		}
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			camera_vel.x = -1;
		}
		if (action == GLFW_RELEASE) {
			camera_vel.x = 0;
		}
	}

	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			camera_vel.y = 1;
		}
		if (action == GLFW_RELEASE) {
			camera_vel.y = 0;
		}
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			camera_vel.x = 1;
		}
		if (action == GLFW_RELEASE) {
			camera_vel.x = 0;
		}
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_vel.z = static_cast<float>(yoffset * 100);
}

int main(int argc, const char * argv[])
{
	if (argc == 1) {
		std::cout << "Usage: AssimpViewer <filename> " << std::endl;
		return -1;
	}

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "AssimpViewer", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	std::string filename = argv[1];
	std::cout << "Loading " << filename << std::endl;

	Shader* shader = new Shader();
	shader->AddComponent("shaders/fs.glsl", GL_FRAGMENT_SHADER);
	shader->AddComponent("shaders/vs.glsl", GL_VERTEX_SHADER);
	shader->Link();

	Model* m = Import::LoadFile(filename);
	m->SetAnimation("Take 001");
	m->SetShader(shader);

	glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 0.0f);
	float model_rotation = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_position += (camera_vel * 0.005f);
		shader->SetMat4("model_matrix", glm::mat4(1));
		glm::mat4 view_matrix = glm::translate(glm::mat4(1), camera_position);
		//view_matrix = glm::rotate(view_matrix, model_rotation, glm::vec3(0, 1, 0));
		shader->SetMat4("view_matrix", view_matrix);
		glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), (float)window_width / window_height, 0.1f, 100.0f);
		shader->SetMat4("projection_matrix", projection_matrix);

		m->Update(0.1);
		m->Render();

		camera_vel.z *= 0.5;
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
