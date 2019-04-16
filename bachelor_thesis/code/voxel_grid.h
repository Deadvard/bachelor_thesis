#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H
#include <glm/glm.hpp>
#include <vector>

struct Isosurface
{
	float distances[1000];
};

struct Voxel
{
	static const int NUM_CORNERS = 8;
	float densities[NUM_CORNERS];
};

struct VoxelData
{
	static const int WIDTH = 10;
	Voxel voxelGrid[WIDTH * WIDTH * WIDTH];

	glm::ivec3 size;

	Isosurface isosurface;
};

void initialize(VoxelData* data);
void densityFunction(VoxelData* data);

#endif