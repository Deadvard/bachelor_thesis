#include "graphics.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

float vertices[] = {
	0.5f,  0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

void initalize(RenderData* data)
{
	data->primaryShader = createShader("resources/shaders/primaryShader.vs", "resources/shaders/primaryShader.fs");
	data->tempTexture = createTexture("resources/textures/bth.jpg");

	glGenVertexArrays(1, &data->tempVAO);
	glBindVertexArray(data->tempVAO);

	glGenBuffers(1, &data->tempVBO);
	glBindBuffer(GL_ARRAY_BUFFER, data->tempVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &data->tempEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->tempEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	data->view = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	data->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
}

void render(const RenderData* data)
{
	glUseProgram(data->primaryShader);
	uniform(data->primaryShader, "view", data->view);
	uniform(data->primaryShader, "projection", data->projection);
	bindTexture(data->tempTexture, 0);
	glBindVertexArray(data->tempVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}