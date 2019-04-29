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
	float offset;
	glm::ivec3 size;
	Isosurface isosurface;
};

void initialize(VoxelData* data);
void densityFunction(VoxelData* data);

void sphere(VoxelData* data, glm::vec3 position, int radius);

#endif