#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

class Renderer
{
public:
	unsigned int VAO;
	unsigned int verticesAmount;
	Shader* shader;
	Texture* texture;
	Renderer(Shader* shader, Texture* texture, float vertices[], GLsizei stride, unsigned int arraySize);
	void draw(glm::mat4 proj, glm::mat4 view, glm::mat4 model, bool lightBounce, glm::vec3 lightPosition, glm::vec3 cameraPosition);
};

#endif
