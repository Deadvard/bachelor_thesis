#version 430

layout(std430, binding = 4) buffer inBuffer
{
	int distances[];
};

layout(std430, packed, binding = 5) buffer histoPyramid
{
	int verticesPerCell[];
};

layout(std140, binding = 6) buffer tables
{
	int edgeTable[256];
	int triTable[256 * 16];
	int vertCountTable[256];
};

vec3 interpolation(vec3 p1, vec3 p2, int v1, int v2)
{
	float f1 = float(v1) / 1000.0;
	float f2 = float(v2) / 1000.0;

	vec3 p;
	float isolevel = 0.0;
	
	if (abs(isolevel - f1) < 0.00001f)
		return p1;
	
	if (abs(isolevel - f2) < 0.00001f)
		return p2;
	
	if (abs(f1 - f2) < 0.00001f)
		return p1;
	
	float mu = (isolevel - f1) / (f2 - f1);

	p.x = p1.x + float(mu) * (p2.x - p1.x);
	p.y = p1.y + float(mu) * (p2.y - p1.y);
	p.z = p1.z + float(mu) * (p2.z - p1.z);
	return p;
}

void main()
{
	uint id = gl_VertexID;
	uint layerSize = 4;
	uint pos = 0;
	uint div = 0;

	for(uint offset = 349520; offset > 0; offset -= layerSize)
	{			
		int cellId = 0;
		for(int i = 0; i < 3 && id > cellId; ++i)
		{
			cellId = verticesPerCell[pos + offset + i];
			id -= cellId;
			div = i + 1;
		}

		layerSize *= 4;
		pos = layerSize / div;
	}

	int xMax = 65;
	int yMax = 65;

	uint indices[8];
	indices[0] = 0 + pos;
	indices[1] = 1 + pos;
	indices[2] = 1 + pos + xMax * yMax;       
	indices[3] = 0 + pos + xMax * yMax;       
	indices[4] = 0 + pos + xMax;             
	indices[5] = 1 + pos + xMax;              
	indices[6] = 1 + pos + xMax + xMax * yMax;
	indices[7] = 0 + pos + xMax + xMax * yMax;

	uint caseCode =
		int(distances[indices[7]] < 0) << 7 |
		int(distances[indices[6]] < 0) << 6 |
		int(distances[indices[5]] < 0) << 5 |
		int(distances[indices[4]] < 0) << 4 |
		int(distances[indices[3]] < 0) << 3 |
		int(distances[indices[2]] < 0) << 2 |
		int(distances[indices[1]] < 0) << 1 |
		int(distances[indices[0]] < 0) << 0;

	float offset = 0.1;
	float fx = (pos % xMax) * offset;
	float fy = ((pos / xMax) % yMax) * offset;
	float fz = (pos / (xMax * yMax)) * offset;
	
	vec3 positions[8];

	positions[0] = vec3(fx, fy, fz);
	positions[1] = vec3(fx + offset, fy, fz);
	positions[2] = vec3(fx + offset, fy, fz + offset);
	positions[3] = vec3(fx, fy, fz + offset);
						 
	positions[4] = vec3(fx, fy + offset, fz);
	positions[5] = vec3(fx + offset, fy + offset, fz);
	positions[6] = vec3(fx + offset, fy + offset, fz + offset);
	positions[7] = vec3(fx, fy + offset, fz + offset);

	int edgeIndex = triTable[caseCode + id * 16];
	vec3 vertex;

	if (edgeIndex == 0)
		vertex = interpolation(positions[0], positions[1], distances[indices[0]], distances[indices[1]]);
	if (edgeIndex == 1)
		vertex = interpolation(positions[1], positions[2], distances[indices[1]], distances[indices[2]]);
	if (edgeIndex == 2)
		vertex = interpolation(positions[2], positions[3], distances[indices[2]], distances[indices[3]]);
	if (edgeIndex == 3)
		vertex = interpolation(positions[3], positions[0], distances[indices[3]], distances[indices[0]]);
	if (edgeIndex == 4)
		vertex = interpolation(positions[4], positions[5], distances[indices[4]], distances[indices[5]]);
	if (edgeIndex == 5)
		vertex = interpolation(positions[5], positions[6], distances[indices[5]], distances[indices[6]]);
	if (edgeIndex == 6)
		vertex = interpolation(positions[6], positions[7], distances[indices[6]], distances[indices[7]]);
	if (edgeIndex == 7)
		vertex = interpolation(positions[7], positions[4], distances[indices[7]], distances[indices[4]]);
	if (edgeIndex == 8)
		vertex = interpolation(positions[0], positions[4], distances[indices[0]], distances[indices[4]]);
	if (edgeIndex == 9)
		vertex = interpolation(positions[1], positions[5], distances[indices[1]], distances[indices[5]]);
	if (edgeIndex == 10)
		vertex = interpolation(positions[2], positions[6], distances[indices[2]], distances[indices[6]]);
	if (edgeIndex == 11)
		vertex = interpolation(positions[3], positions[7], distances[indices[3]], distances[indices[7]]);

	gl_Position = vec4(vertex, 1.0);
}