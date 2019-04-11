#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>

struct Points
{
	unsigned int vao, vbo;
	glm::mat4 model;
};

struct MarchingCubes
{
	unsigned int vao, vbo;
	unsigned int numTriangles;
};

struct RenderData
{
	unsigned int primaryShader;
	unsigned int pointShader;
	unsigned int uniformBuffer;
	Points points;
	MarchingCubes marchingCubes;

	glm::mat4 view;
	glm::mat4 projection;
};

void initalize(RenderData* data);
void render(const RenderData* data);
void update(RenderData* data);

#endif
