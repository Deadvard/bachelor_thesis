#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H
#include <glm/glm.hpp>
#include <vector>

struct Isosurface
{
	char* distances;
};

struct VoxelData
{
	glm::ivec3 size;
	Isosurface isosurface;
};

void initialize(VoxelData* data);
void densityFunction(VoxelData* data);

#endif