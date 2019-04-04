#include "graphics.h"

#include <glad/glad.h>

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

	glGenVertexArrays(1, &data->temp_vao);
	glBindVertexArray(data->temp_vao);
	glGenBuffers(1, &data->temp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, data->temp_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &data->temp_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->temp_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void render(const RenderData* data)
{
	glUseProgram(data->primaryShader);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->temp_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}