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

	unsigned int ptVao, ptVbo;
	unsigned int numPoints;

	unsigned int marchingCubesShader;

	unsigned int histoPyramidShader;

	unsigned int computeShader;
	int* tempDistances;
	unsigned int inputBuffer;
	unsigned int outputBuffer;
	unsigned int indirectBuffer;

	unsigned int tableBuffer;

	unsigned int triTableBuffer;
	unsigned int vertTableBuffer;


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
void render(RenderData* data, VoxelData* voxelData, glm::vec3* camPos, glm::mat4* model, int id);

#endif
