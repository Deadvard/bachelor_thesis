#include "graphics.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void initalize(RenderData* data)
{
	data->primaryShader = createShader("resources/shaders/primary_shader.vs", "resources/shaders/primary_shader.fs");
	data->pointShader = createShader("resources/shaders/point_shader.vs", "resources/shaders/point_shader.fs");

	data->points.model = glm::mat4(1.f);
	data->points.model[3] = glm::vec4(0,1,0,1);

	data->view = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	data->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	glGenBuffers(1, &data->uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, data->uniformBuffer, 0, 2 * sizeof(glm::mat4));
}

void render(const RenderData* data)
{
	glUseProgram(data->primaryShader);
	uniform(data->primaryShader, "view", data->view);
	uniform(data->primaryShader, "projection", data->projection);
	glBindVertexArray(data->meshes.vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, (int)data->meshes.positions.size());
	
	glUseProgram(data->pointShader);
	uniform(data->pointShader, "model", data->points.model);
	uniform(data->pointShader, "view", data->view);
	uniform(data->pointShader, "projection", data->projection);
	glBindVertexArray(data->points.vao);
	glPointSize(25.0f);
	glDrawArrays(GL_POINTS, 0, 125);
}

void update(RenderData* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(data->view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(data->projection));
}
