#include "voxel_grid.h"
#include <iostream>
void initialize(VoxelData* data)
{
	for (int i = 0; i < data->NUM_CHUNKS; ++i)
	{
		data->voxelGrid[i].id = i;
		for(int j = 0; j < 8; ++j)
			data->voxelGrid[i].densities[j] = 0.f;
	}

	densityFunction(data);

	for (int i = 0; i < data->NUM_CHUNKS; i++)
	{
		for(int j = 0; j < 8; j++)
			std::cout << data->voxelGrid[i].densities[j] << '\n';
	}

	for (auto& voxel : data->voxels.densities)
	{
		voxel = 0.f;
	}

	sphere(&data->voxels, 2.0f);
}

void densityFunction(VoxelData * data)
{
	float radius = 2.0f;
	glm::vec3 pos = glm::vec3(4,4,4);

	for (int i = 0; i < data->NUM_CHUNKS; ++i)
	{
		int x = i % data->NUM_CHUNKS;
		int y = (i / data->NUM_CHUNKS) % data->NUM_CHUNKS;
		int z = i / (data->NUM_CHUNKS * data->NUM_CHUNKS);

		glm::vec3 positions[8];
		positions[0] = glm::vec3(x, y, z);
		positions[1] = glm::vec3(x + 1, y, z);
		positions[2] = glm::vec3(x + 1, y, z + 1);
		positions[3] = glm::vec3(x, y, z + 1);

		positions[4] = glm::vec3(x, y + 1, z);
		positions[5] = glm::vec3(x + 1, y + 1, z);
		positions[6] = glm::vec3(x + 1, y + 1, z + 1);
		positions[7] = glm::vec3(x, y + 1, z + 1);

		float sqrRad = radius * radius;

		for (int j = 0; j < 8; j++)
		{
			glm::vec3 newPos = pos - positions[j];
			float sqrDist = newPos.x * newPos.x * newPos.y * newPos.y * newPos.z * newPos.z;
			data->voxelGrid[i].densities[j] = sqrRad - sqrDist;
		}
	}
}

void sphere(Isosurface* isosurface, float radius)
{
	for (int i = 0; i < 125; ++i)
	{
		int x = i % 5;
		int y = (i / 5) % 5;
		int z = i / (5 * 5);

		float sqrDist = x * x + y * y + z * z;
		float sqrRad = radius * radius;
		isosurface->densities[i] = sqrDist - sqrRad;
	}
}
