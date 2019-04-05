#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>

struct Meshes
{
	std::vector<glm::vec3> positions;
	unsigned int vao, vbo, instanceVbo;
};

struct RenderData
{
	unsigned int primaryShader;
	unsigned int uniformBuffer;
	Meshes meshes;

	glm::mat4 view;
	glm::mat4 projection;
};

void initalize(RenderData* data);
void render(const RenderData* data);
void update(RenderData* data);

#endif
