#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Shader* _shader, Texture* _texture, float vertices[], GLsizei stride, unsigned int arraySize)
{
	shader = _shader;
	texture = _texture;

	VAO = 0;
	//unsigned int EBO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// why is this a one????
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	verticesAmount = arraySize / (sizeof(float) * stride);
}

void Renderer::draw(glm::mat4 proj, glm::mat4 view, glm::mat4 model, bool lightBounce, glm::vec3 lightPosition, glm::vec3 cameraPosition)
{
	if (shader != nullptr)
	{
		shader->use();
		shader->setMatrix4f("model", glm::value_ptr(model));
		shader->setMatrix4f("view", glm::value_ptr(view));
		shader->setMatrix4f("projection", glm::value_ptr(proj));

		// TODO: This should not be here
		if (lightBounce)
		{
			shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			shader->setVec3("lightPosition", lightPosition);
			shader->setVec3("viewPos", cameraPosition);
		}

	}
	
	if (texture != nullptr)
		texture->activateTextureAs(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, verticesAmount);
}
