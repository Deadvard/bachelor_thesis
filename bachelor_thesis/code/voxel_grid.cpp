#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	densityFunction(data);
}

void densityFunction(VoxelData * data)
{
	data->size = glm::ivec3(10, 10, 10);
	
	float radius = 2.f;
	glm::vec3 pos = glm::vec3(5,5,5);
	
	for (int i = 0; i < data->WIDTH * data->WIDTH * data->WIDTH; ++i)
	{
		int x = i % data->WIDTH;
		int y = (i / data->WIDTH) % data->WIDTH;
		int z = i / (data->WIDTH * data->WIDTH);

		glm::vec3 positions[data->voxelGrid[i].NUM_CORNERS];
		positions[0] = glm::vec3(x, y, z);
		positions[1] = glm::vec3(x + 1, y, z);
		positions[2] = glm::vec3(x + 1, y, z + 1);
		positions[3] = glm::vec3(x, y, z + 1);

		positions[4] = glm::vec3(x, y + 1, z);
		positions[5] = glm::vec3(x + 1, y + 1, z);
		positions[6] = glm::vec3(x + 1, y + 1, z + 1);
		positions[7] = glm::vec3(x, y + 1, z + 1);

		glm::ivec3 size = data->size;

		if (x != size.x && y != size.y && z != size.z)
		{
			int indices[8];
			indices[0] = 0 + i;
			indices[1] = 1 + i;
			indices[2] = 1 + i + size.x * size.y;
			indices[3] = 0 + i + size.x * size.y;
			indices[4] = 0 + i + size.x;
			indices[5] = 1 + i + size.x;
			indices[6] = 1 + i + size.x + size.x * size.y;
			indices[7] = 0 + i + size.x + size.x * size.y;

			for (int j = 0; j < data->voxelGrid[i].NUM_CORNERS; ++j)
			{
				glm::vec3 newPos = pos - positions[j];
				float dist = std::sqrt(newPos.x * newPos.x + newPos.y * newPos.y + newPos.z * newPos.z);
				data->isosurface.distances[indices[j]] = radius - dist;
			}
		}
	}
}