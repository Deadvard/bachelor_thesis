#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H
#include <glm/glm.hpp>
#include <vector>

struct Voxel
{
	static const int NUM_CORNERS = 8;
	float densities[NUM_CORNERS];
};

struct VoxelData
{
	static const int WIDTH = 10;
	Voxel voxelGrid[WIDTH * WIDTH * WIDTH];
};

void initialize(VoxelData* data);
void densityFunction(VoxelData* data);

#endif