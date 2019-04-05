#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	data->grid.resize(500);
	for (auto& cell : data->grid)
	{
		static int x = 0;
		static int z = 0;
		cell.position = glm::vec3(x++, 0, z++);
	}
}
