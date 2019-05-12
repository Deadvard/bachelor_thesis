#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	data->distancesSize = glm::ivec3(65, 65, 65);
	data->offset = 0.1f;
	data->isosurface.distances = new char[data->distancesSize.x * data->distancesSize.y * data->distancesSize.z];
	memset(data->isosurface.distances, 0, data->distancesSize.x * data->distancesSize.y * data->distancesSize.z);
	sphere(data, glm::vec3(4.0f), 1.0f);
	
	AABB box;
	box.position = glm::vec3(2.0f);
	box.size = glm::vec3(0.5f);
	aabb(data, box);
}

void sphere(VoxelData* data, glm::vec3 position, float radius)
{
	/*glm::ivec3 first = 10.0f * (position - radius);
	glm::ivec3 last = 10.0f * (position + radius);
	glm::ivec3 size = data->distancesSize;

	first = glm::max(first, glm::ivec3(0));
	last = glm::min(last, size);

	for (int z = first.z; z < last.z; ++z)
	for (int y = first.y; y < last.y; ++y)
	for (int x = first.x; x < last.x; ++x)
	{
		int i = x + size.x * y + size.x * size.y * z;*/
		
	glm::ivec3 size = data->distancesSize;
	for (int i = 0; i < size.x * size.y * size.z; ++i)
	{
		int x = i % size.x;
		int y = (i / size.x) % size.y;
		int z = i / (size.x * size.y);

		float fx = (float)x * data->offset;
		float fy = (float)y * data->offset;
		float fz = (float)z * data->offset;

		glm::vec3 newPos = position - glm::vec3(fx, fy, fz);
		int dist = (int)(1000.0f * (radius - glm::length(newPos)));

		if (dist < 100)
		{
			if (dist < -100) dist = -100;
			data->isosurface.distances[i] = (char)dist;
		}
	}
}

void aabb(VoxelData* data, const AABB& aabb)
{
	glm::ivec3 first = 10.0f * (aabb.position - aabb.size);
	glm::ivec3 last = 10.0f * (aabb.position + aabb.size);
	glm::ivec3 size = data->distancesSize;

	first = glm::max(first, glm::ivec3(0));
	last = glm::min(last, size);

	for (int z = first.z; z < last.z; ++z)
	for (int y = first.y; y < last.y; ++y)
	for (int x = first.x; x < last.x; ++x)
	{
		int i = x + size.x * y + size.x * size.y * z;

		float fx = (float)x * data->offset;
		float fy = (float)y * data->offset;
		float fz = (float)z * data->offset;

		glm::vec3 newPos = glm::vec3(fx, fy, fz);
		newPos = closest_point(aabb, newPos) - newPos;
		
		int dist = (int)(1000.0f * glm::length(newPos));

		if (dist < 100)
		{
			if (dist < -100) dist = -100;
			data->isosurface.distances[i] = (char)dist;
		}
	}
}

glm::vec3 closest_point(const AABB& aabb, const glm::vec3& point)
{
	glm::vec3 result = point;
	glm::vec3 min = get_min(aabb);
	glm::vec3 max = get_max(aabb);

	result.x = result.x < min.x ? min.x : result.x;
	result.y = result.y < min.y ? min.y : result.y;
	result.z = result.z < min.z ? min.z : result.z;

	result.x = result.x > max.x ? max.x : result.x;
	result.y = result.y > max.y ? max.y : result.y;
	result.z = result.z > max.z ? max.z : result.z;

	return result;
}

glm::vec3 get_min(const AABB& aabb)
{
	glm::vec3 p1 = aabb.position + aabb.size;
	glm::vec3 p2 = aabb.position - aabb.size;

	return glm::vec3(
		glm::min(p1.x, p2.x),
		glm::min(p1.y, p2.y),
		glm::min(p1.z, p2.z));
}

glm::vec3 get_max(const AABB& aabb)
{
	glm::vec3 p1 = aabb.position + aabb.size;
	glm::vec3 p2 = aabb.position - aabb.size;

	return glm::vec3(
		glm::max(p1.x, p2.x),
		glm::max(p1.y, p2.y),
		glm::max(p1.z, p2.z));
}