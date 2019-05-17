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
	glm::ivec3 distancesSize;
	Isosurface isosurface[4];
};

void initialize(VoxelData* data);

struct AABB { glm::vec3 position; glm::vec3 size; };

void sphere(VoxelData* data, glm::vec3 position, float radius, int id);
void aabb(VoxelData* data, const AABB& aabb, int id);


glm::vec3 closest_point(const AABB& aabb, const glm::vec3& point);
glm::vec3 get_min(const AABB& aabb);
glm::vec3 get_max(const AABB& aabb);


#endif