#include "dc.h"

#include <math.h>

/*void fDensity(
	float x0, float y0, float z0,
	float dz, int num_points, float* densities) {}

void fNormal(
	const glm::vec3* point, glm::vec3* normal) {}

static const float VOXELSIZE = 0.1f;

void computePoints(Isomesher_DC* dc, Row* row)
{
	float x0 = row->v.x;
	float y0 = row->v.y;
	float z0 = row->v.z;
	float dz = VOXELSIZE;

	for (int x = 0; x < dc->xSize; ++x)
	{
		float* densities = &row->densities[x * dc->zSize];
		fDensity(x0, y0, z0, dz, dc->zSize, densities);

		glm::vec3* points = &row->points[x * dc->zSize];

		for (int z = 0; z < dc->zSize; ++z)
		{
			densities[z] += 1e-4f;
			points[z] = glm::vec3(x0, y0, z0 + z * dz);
		}
		x0 += VOXELSIZE;
	}
}

void computeCubes(Isomesher_DC* dc, Row* rows[2])
{
	int xSize_1 = dc->xSize - 1;
	int zSize_1 = dc->zSize - 1;

	for (int x = 0; x < xSize_1; ++x)
	{
		for (int z = 0; z < zSize_1; ++z)
		{
			Cube* cube = &rows[0]->cubes[x * dc->zSize + z];

#define DENSITY_VAL(xi,yi,zi) rows[yi]->densities[(x + xi) * dc->zSize + (z + zi)]
#define POINT_PTR(xi,yi,zi) &rows[yi]->points[(x + xi) * dc->zSize + (z + zi)]
#define CORNER(n,xi,yi,zi) \
	corners[n].density = DENSITY_VAL(xi, yi, zi); \
	corners[n].v = POINT_PTR(xi, yi, zi);

			Point corners[8];
			CORNER(0, 0,0,0);
			CORNER(1, 1,0,0);
			CORNER(2, 1,1,0);
			CORNER(3, 0,1,0);
			CORNER(4, 0,0,1);
			CORNER(5, 1,0,1);
			CORNER(6, 1,1,1);
			CORNER(7, 0,1,1);
#undef CORNER
#undef POINT_PTR
#undef DENSITY_VAL

			int index = 0;

#define IF_DENSITY(i) if (!fsign(corners[i].density)) index |= (1 << i)

			IF_DENSITY(0);
			IF_DENSITY(1);
			IF_DENSITY(2);
			IF_DENSITY(3);
			IF_DENSITY(4);
			IF_DENSITY(5);
			IF_DENSITY(6);
			IF_DENSITY(7);

#undef IF_DENSITY

			if (edgeTable[index] != 0)
			{
				cube->index = index;
				generateVertex(dc, cube, corners);
			}
			else
			{
				cube->index = 0;
				cube->meshPoint = 0;
			}
		}
	}
}

void generateVertex(Isomesher_DC* dc, Cube* cube, Point corners[8])
{
	static int intersections[12][2] = {
	   { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
	   { 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
	};

	int edgeInfo = edgeTable[cube->index];

	Point points[12];
	glm::vec3 points_v[12];
	glm::vec3 normals[12];
	glm::vec3 massPoint;
	int numIntersections;

	for (int i = 0; i < 12; ++i)
	{
		if (edgeInfo & (1 << i))
		{
			int n1 = intersections[i][0];
			int n2 = intersections[i][1];

			if (glm::abs(corners[n1].density) < 1e-3)
			{
				points[i] = corners[n1];
			}
			else if (glm::abs(corners[n2].density) < 1e-3)
			{
				points[i] = corners[n2];
			}
			else if (glm::abs(corners[n1].density - corners[n2].density) < 1e-3)
			{
				points[i] = corners[n1];
			}
			else
			{
				points[i].v = &points_v[i];
				glm::vec3 v_diff = glm::abs(*corners[n1].v - *corners[n2].v);

				if (v_diff.x > 1e-3)
				{
					intersect_xaxis(&corners[n1], &corners[n2], &points[i]);
				}
				else if (v_diff.y > 1e-3)
				{
					intersect_yaxis(&corners[n1], &corners[n2], &points[i]);
				}
				else if (v_diff.z > 1e-3)
				{
					intersect_zaxis(&corners[n1], &corners[n2], &points[i]);
				}
			}

			fNormal(points[i].v, &normals[i]);
			massPoint += *points[i].v;
			++numIntersections;
		}
	}

	massPoint /= (float)numIntersections;
}*/
