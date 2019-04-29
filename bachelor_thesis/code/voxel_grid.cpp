#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	data->size = glm::ivec3(64, 64, 64);
	data->offset = 0.1f;
	data->isosurface.distances = new char[data->size.x * data->size.y * data->size.z];
	densityFunction(data);

	memset(data->isosurface.distances, 0, data->size.x * data->size.y * data->size.z);
}

void densityFunction(VoxelData * data)
{
	int radius = 1000;
	
	for (int i = 0; i < data->size.x * data->size.y * data->size.z; ++i)
	{
		int x = i % data->size.x;
		int y = (i / data->size.x) % data->size.y;
		int z = i / (data->size.x * data->size.y);

		float fx = (float)x * data->offset;
		float fy = (float)y * data->offset;
		float fz = (float)z * data->offset;

		glm::vec3 newPos = glm::vec3(fx, fy, fz);
		int dist = radius - (int)(1000 * glm::length(newPos));

		if (dist < 255 && dist > -255)
		{
			printf("dist = %i\n", dist);
			data->isosurface.distances[i] = dist;
		}
	}
}