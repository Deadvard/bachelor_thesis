#version 430

layout(std430, binding = 4) buffer inBuffer
{
	int distances[];
};

layout(std430, packed, binding = 5) buffer histoPyramid
{
	int verticesPerCell[];
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

	uint caseCode =
	int(distances[0 + id                     ] < 0) << 0 |
	int(distances[1 + id                     ] < 0) << 1 |
	int(distances[1 + id + xMax * yMax       ] < 0) << 2 |
	int(distances[0 + id + xMax * yMax       ] < 0) << 3 |
	int(distances[0 + id + xMax              ] < 0) << 4 |
	int(distances[1 + id + xMax              ] < 0) << 5 |
	int(distances[1 + id + xMax + xMax * yMax] < 0) << 6 |
	int(distances[0 + id + xMax + xMax * yMax] < 0) << 7;

	vec3 vertices[12];
	if(edgeTable[pos]&1)
	{
		vertices[0] = interpolation(positions[0], positions[1], distances[indices[0]], distances[indices[1]]);
	}


}