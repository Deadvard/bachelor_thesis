#include "voxel_grid.h"

void initialize(VoxelData* data)
{
	data->size = glm::ivec3(64, 64, 64);
	data->offset = 0.1f;
	data->isosurface.distances = new char[data->size.x * data->size.y * data->size.z];
	densityFunction(data);
}

void densityFunction(VoxelData * data)
{
	float radius = 2.f;
	glm::vec3 pos = glm::vec3(5,5,5);
	
	for (int i = 0; i < data->size.x * data->size.y * data->size.z; ++i)
	{
		int x = i % data->size.x;
		int y = (i / data->size.x) % data->size.y;
		int z = i / (data->size.x * data->size.y);

		float fx = (float)x * data->offset;
		float fy = (float)y * data->offset;
		float fz = (float)z * data->offset;

		glm::vec3 positions[8]; // 8 corners
		positions[0] = glm::vec3(fx, fy, fz);
		positions[1] = glm::vec3(fx + data->offset, fy, fz);
		positions[2] = glm::vec3(fx + data->offset, fy, fz + data->offset);
		positions[3] = glm::vec3(fx, fy, fz + data->offset);
								 
		positions[4] = glm::vec3(fx, fy + data->offset, fz);
		positions[5] = glm::vec3(fx + data->offset, fy + data->offset, fz);
		positions[6] = glm::vec3(fx + data->offset, fy + data->offset, fz + data->offset);
		positions[7] = glm::vec3(fx, fy + data->offset, fz + data->offset);

		if (x < data->size.x - 1 && y < data->size.y - 1 && z < data->size.z - 1)
		{
			int indices[8];
			indices[0] = 0 + i;
			indices[1] = 1 + i;
			indices[2] = 1 + i + data->size.x * data->size.y;
			indices[3] = 0 + i + data->size.x * data->size.y;
			indices[4] = 0 + i + data->size.x;
			indices[5] = 1 + i + data->size.x;
			indices[6] = 1 + i + data->size.x + data->size.x * data->size.y;
			indices[7] = 0 + i + data->size.x + data->size.x * data->size.y;

			for (int j = 0; j < 8; ++j) // 8 corners
			{
				glm::vec3 newPos = pos - positions[j];
				float dist = std::sqrt(newPos.x * newPos.x + newPos.y * newPos.y + newPos.z * newPos.z);
				data->isosurface.distances[indices[j]] = radius - dist;
			}
		}
	}
}