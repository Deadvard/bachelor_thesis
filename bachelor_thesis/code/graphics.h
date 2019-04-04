#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"

struct RenderData
{
	unsigned int primaryShader;
	unsigned int tempTexture;

	unsigned int tempVAO;
	unsigned int tempVBO;
	unsigned int tempEBO;

	glm::mat4 view;
	glm::mat4 projection;
};

void initalize(RenderData* data);
void render(const RenderData* data);

#endif
