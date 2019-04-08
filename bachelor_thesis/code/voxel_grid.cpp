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
}
