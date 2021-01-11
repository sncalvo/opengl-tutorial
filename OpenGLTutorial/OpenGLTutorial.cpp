/* Standard imports */
#include <iostream>

/* GLM imports */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* OpenGL imports */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

/* Constants */
constexpr auto WINDOW_HEIGTH = 600;
constexpr auto WINDOW_WIDTH = 800;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGTH / 2.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Initialize GLFW window */
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGTH, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Initialize GLAD */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader objectShader("lightTest.vs", "lightTest.fs");
	Shader lightShader("lightSource.vs", "lightSource.fs");
	//Texture texture("container.jpg");

	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	//	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	//	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	//};
	//Renderer box(&firstShader, &texture, vertices, 5, sizeof(vertices));

	float vertices[] = {
		// positions // normals // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	Renderer box(&objectShader, nullptr, vertices, 8, sizeof(vertices));
	Renderer light(&lightShader, nullptr, vertices, 8, sizeof(vertices));

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	/* Render loop */
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
			100.0f);

		float angle = glm::radians((float)glfwGetTime() * (float)glfwGetTime());
		angle = glm::cos(angle);
		glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f) * glm::vec3(1.0f, 1.0f, 0.0f) * angle;
		
		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::rotate(cubeModel, glm::radians((float)glfwGetTime() * (float)glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));
		cubeModel = glm::scale(cubeModel, glm::vec3(0.5, 0.5, 0.5));

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(cubeModel);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));
			box.draw(projection, view, model, true, lightPos, camera.Position);
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		light.draw(projection, view, model, false, lightPos, camera.Position);
		// events and buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
