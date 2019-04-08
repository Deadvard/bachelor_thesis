#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	data->grid.resize(625);
	for (auto& cell : data->grid)
	{
		static int x = 0;
		static int z = 0;
		cell.position = glm::vec3(x++, 0, z);
		if (x % 25 == 0)
		{
			x = 0;
			z++;
		}
	}

	for (auto& voxel : data->voxels.densities)
	{
		voxel = 0.f;
	}

	sphere(&data->voxels, 2.0f);
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
