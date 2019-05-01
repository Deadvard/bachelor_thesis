#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>
#include "voxel_grid.h"
#include "tables.h"

struct MarchingCubes
{
	unsigned int vao, vbo, ebo;
	unsigned int numTriangles;

	unsigned int pt_vao, pt_vbo;
	unsigned int numPoints;

	unsigned int computeShader;
	int* tempDistances;
	unsigned int inputBuffer;
	unsigned int outputBuffer;

	unsigned int tableBuffer;


};

struct RenderData
{
	unsigned int primaryShader;
	unsigned int pointShader;
	unsigned int uniformBuffer;
	MarchingCubes marchingCubes;

	glm::mat4 view;
	glm::mat4 projection;
};

void initalize(RenderData* data);
void render(const RenderData* data);
void update(RenderData* data, VoxelData* voxelData);

#endif
