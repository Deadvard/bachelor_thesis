#include "graphics.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void initializeMarchingCubes(RenderData* data);

void initalize(RenderData* data)
{
	data->primaryShader = createShader("resources/shaders/primary_shader.vs", "resources/shaders/primary_shader.fs");
	data->pointShader = createShader("resources/shaders/point_shader.vs", "resources/shaders/point_shader.fs");
	data->view = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	data->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	glGenBuffers(1, &data->uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, data->uniformBuffer, 0, 2 * sizeof(glm::mat4));

	initializeMarchingCubes(data);
	data->marchingCubes.tempDistances = new int[64 * 64 * 64];
}

void render(const RenderData* data)
{
	glUseProgram(data->primaryShader);
	uniform(data->primaryShader, "view", data->view);
	uniform(data->primaryShader, "projection", data->projection);
	glBindVertexArray(data->marchingCubes.vao);
	glDrawArrays(GL_TRIANGLES, 0, data->marchingCubes.numTriangles * 3);

	glUseProgram(data->pointShader);
	uniform(data->pointShader, "model", glm::mat4(1.f));
	uniform(data->pointShader, "view", data->view);
	uniform(data->pointShader, "projection", data->projection);
	glBindVertexArray(data->marchingCubes.pt_vao);
	glPointSize(2.f);
	glDrawArrays(GL_POINTS, 0, data->marchingCubes.numPoints);
}
#include <iostream>

void update(RenderData* data, VoxelData* voxelData)
{
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(data->view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(data->projection));

	for(int i = 0; i < 64 * 64 * 64; ++i)
		data->marchingCubes.tempDistances[i] = (int)voxelData->isosurface.distances[i];

	glUseProgram(data->marchingCubes.computeShader);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.tableBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int) * 256, &edgeTable[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 256, sizeof(int) * 256 * 16, &triTable[0][0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.inputBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int) * 64 * 64 * 64, &data->marchingCubes.tempDistances[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int) * 64 * 64 * 64, nullptr);

	glDispatchCompute(8,8,8);
	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	GLint*ptr = (GLint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	for (int i = 0; i < 64 * 64 * 64; ++i)
	{
		if(ptr[i] != 0)
			std::cout << ptr[i] << '\n';
	}
	
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void initializeMarchingCubes(RenderData * data)
{
	data->marchingCubes.computeShader = createShader("resources/shaders/compute_shader.comp");

	glGenBuffers(1, &data->marchingCubes.tableBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.tableBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 256 + sizeof(int) * 256 * 12, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, data->uniformBuffer);

	glGenBuffers(1, &data->marchingCubes.inputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.inputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 64 * 64 * 64, nullptr, GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, data->marchingCubes.inputBuffer);

	glGenBuffers(1, &data->marchingCubes.outputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 64 * 64 * 64, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, data->marchingCubes.outputBuffer);
}
