#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H
#include <glm/glm.hpp>
#include <vector>

struct Voxel
{
	//attributes
	bool isEmpty;
	bool hasMaterial;
	bool hasUV;

	glm::vec3 position;

	//more stuff here
};

struct VoxelData
{
	std::vector<Voxel> grid;
};

void initialize(VoxelData* data);

#endif