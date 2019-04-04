#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "shader.h"
#include "texture.h"

struct RenderData
{
	unsigned int primaryShader;
	unsigned int tempTexture;

	unsigned int tempVAO;
	unsigned int tempVBO;
	unsigned int tempEBO;
};

void initalize(RenderData* data);
void render(const RenderData* data);

#endif
