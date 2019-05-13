#include "graphics.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void initializeMarchingCubes(RenderData* data);

void initalize(RenderData* data)
{
	//data->primaryShader = createShader("resources/shaders/primary_shader.vs", "resources/shaders/primary_shader.fs");
	//data->pointShader = createShader("resources/shaders/point_shader.vs", "resources/shaders/point_shader.fs");
	data->view = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	data->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	glGenBuffers(1, &data->uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, data->uniformBuffer, 0, 2 * sizeof(glm::mat4));

	initializeMarchingCubes(data);
	data->marchingCubes.tempDistances = new int[65 * 65 * 65];

	glGenVertexArrays(1, &data->marchingCubes.vao);
}

void render(const RenderData* data, const glm::vec3* camPos)
{
	//glUseProgram(data->primaryShader);
	//glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	//glBindVertexArray(data->marchingCubes.vao);
	//glDrawArrays(GL_TRIANGLES, 0, data->marchingCubes.numTriangles * 3);
	
	//glUseProgram(data->pointShader);
	//uniform(data->pointShader, "model", glm::mat4(1.f));
	//uniform(data->pointShader, "view", data->view);
	//uniform(data->pointShader, "projection", data->projection);
	//glBindVertexArray(data->marchingCubes.ptVao);
	//glPointSize(2.f);
	//glDrawArrays(GL_POINTS, 0, data->marchingCubes.numPoints);
	
	glUseProgram(data->marchingCubes.marchingCubesShader);
	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.triTableBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.inputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, data->marchingCubes.indirectBuffer);
	glm::mat4 model(1.f);
	//model[3] = glm::vec4(6.4, 0, 0, 1);
	uniform(data->marchingCubes.marchingCubesShader, "model", model);
	uniform(data->marchingCubes.marchingCubesShader, "camPos", camPos);
	glBindVertexArray(data->marchingCubes.vao);
	glDrawArraysIndirect(GL_TRIANGLES, 0);
}

void update(RenderData* data, VoxelData* voxelData)
{
	for(int i = 0; i < 65 * 65 * 65; ++i)
		data->marchingCubes.tempDistances[i] = int(voxelData->isosurface.distances[i]);

	glBindBuffer(GL_UNIFORM_BUFFER, data->uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(data->view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(data->projection));

	glUseProgram(data->marchingCubes.computeShader);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.vertTableBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.inputBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLbyte) * 65 * 65 * 65, &voxelData->isosurface.distances[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glDispatchCompute(64,64,64);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	glUseProgram(data->marchingCubes.histoPyramidShader);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.indirectBuffer);
	int offset = 0;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(64, 64, 16);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	offset += 64 * 64 * 64;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(64, 16, 16);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	offset += 64 * 64 * 16;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(16, 16, 16);	
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 64 * 16 * 16;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(16, 16, 4);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 16 * 16 * 16;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(16, 4, 4);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 16 * 16 * 4;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(4, 4, 4);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 16 * 4 * 4;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(4, 4, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 4 * 4 * 4;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(4, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	offset += 4 * 4 * 1;
	uniform(data->marchingCubes.histoPyramidShader, "offset", offset);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);
}

void initializeMarchingCubes(RenderData * data)
{
	data->marchingCubes.marchingCubesShader = createShader("resources/shaders/char_vertex.vs", "resources/shaders/geometry_shader.gs", "resources/shaders/fragment_shader.fs");
	data->marchingCubes.computeShader = createShader("resources/shaders/char_unpack.comp");
	data->marchingCubes.histoPyramidShader = createShader("resources/shaders/histopyramid_builder.comp");

	glGenBuffers(1, &data->marchingCubes.triTableBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.triTableBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 256 * 16, &gpuTriTable[0], GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data->marchingCubes.triTableBuffer);

	glGenBuffers(1, &data->marchingCubes.vertTableBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.vertTableBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 256 * 16, &vertCountTable[0], GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data->marchingCubes.vertTableBuffer);

	glGenBuffers(1, &data->marchingCubes.inputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.inputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLbyte) * 65 * 65 * 65, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, data->marchingCubes.inputBuffer);

	int bufferSize = 64 * 64 * 64;
	bufferSize += 64 * 64 * 16;
	bufferSize += 64 * 16 * 16;
	bufferSize += 16 * 16 * 16;
	bufferSize += 16 * 16 * 4;
	bufferSize += 16 * 4 * 4;
	bufferSize += 4 * 4 * 4;
	bufferSize += 4 * 4 * 1;
	bufferSize += 4 * 1 * 1;
	bufferSize *= sizeof(int);

	glGenBuffers(1, &data->marchingCubes.outputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.outputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, data->marchingCubes.outputBuffer);

	glGenBuffers(1, &data->marchingCubes.indirectBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data->marchingCubes.indirectBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 4, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, data->marchingCubes.indirectBuffer);
}
