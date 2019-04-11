#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H
#include <glm/glm.hpp>
#include <vector>

struct Isosurface
{
	float densities[125];
};

struct Voxel
{
	int id;
	float densities[8];
};

struct VoxelData
{
	static const int NUM_CHUNKS = 10;
	Voxel voxelGrid[10 * 10 * 10];
	Isosurface voxels;
};

void initialize(VoxelData* data);

void densityFunction(VoxelData* data);
void sphere(Isosurface* isosurface, float radius);


#endif