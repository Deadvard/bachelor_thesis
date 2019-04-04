#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "shader.h"
#include "texture.h"

struct RenderData
{
	unsigned int primaryShader;
	unsigned int tempTexture;

	unsigned int temp_vao;
	unsigned int temp_vbo;
	unsigned int temp_ebo;
};

void initalize(RenderData* data);
void render(const RenderData* data);

#endif
