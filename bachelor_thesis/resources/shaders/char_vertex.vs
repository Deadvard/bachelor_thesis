#version 430

layout(std140, binding = 0) uniform matrices
{
	uniform mat4 view;
	uniform mat4 projection;
};

layout(std430, binding = 1) buffer table
{
	int triTable[256 * 16];
};

layout(std430, binding = 3) buffer inBuffer
{
	int distances[];
};

layout(std430, binding = 4) buffer histoPyramid
{
	int verticesPerCell[];
};

out vData
{
    vec3 worldPosition;
};

uniform mat4 model;

vec3 interpolation(vec3 p1, vec3 p2, int v1, int v2)
{	
	if (v1 == 0) return p1;	
	if (v2 == 0) return p2;	
	if (v1 - v2 == 0) return p1;

	float f1 = float(v1) / 1000.0f;
	float f2 = float(v2) / 1000.0f;
		
	float mu = -f1 / (f2 - f1);
	vec3 p;
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);
	return p;
}

void main()
{
	uint id = gl_VertexID;
    int pos = 0;
    int layerSize = 4;

	for(int offset = 349520; offset >= 0; offset -= layerSize)
	{			
		pos *= 4;
		
		for(int i = 0; i < 4; ++i)
		{
			int cellId = verticesPerCell[pos + offset + i];
			if (id >= cellId)
			{
				id -= cellId;
			}
			else
			{
				pos += i;
				break;
			}
			
		}
		
		layerSize *= 4;			
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

	int dist[8];
	dist[0] = (distances[indices[0] / 4] >> ((indices[0] % 4) * 8)) & 255;
	dist[1] = (distances[indices[1] / 4] >> ((indices[1] % 4) * 8)) & 255; 
	dist[2] = (distances[indices[2] / 4] >> ((indices[2] % 4) * 8)) & 255;
	dist[3] = (distances[indices[3] / 4] >> ((indices[3] % 4) * 8)) & 255;
	dist[4] = (distances[indices[4] / 4] >> ((indices[4] % 4) * 8)) & 255;
	dist[5] = (distances[indices[5] / 4] >> ((indices[5] % 4) * 8)) & 255;
	dist[6] = (distances[indices[6] / 4] >> ((indices[6] % 4) * 8)) & 255;
	dist[7] = (distances[indices[7] / 4] >> ((indices[7] % 4) * 8)) & 255;
	
	dist[0] = bitfieldExtract(dist[0], 0, 8);
	dist[1] = bitfieldExtract(dist[1], 0, 8);
	dist[2] = bitfieldExtract(dist[2], 0, 8);
	dist[3] = bitfieldExtract(dist[3], 0, 8);
	dist[4] = bitfieldExtract(dist[4], 0, 8);
	dist[5] = bitfieldExtract(dist[5], 0, 8);
	dist[6] = bitfieldExtract(dist[6], 0, 8);
	dist[7] = bitfieldExtract(dist[7], 0, 8);

	uint caseCode =
		int(dist[7] < 0) << 7 |
		int(dist[6] < 0) << 6 |
		int(dist[5] < 0) << 5 |
		int(dist[4] < 0) << 4 |
		int(dist[3] < 0) << 3 |
		int(dist[2] < 0) << 2 |
		int(dist[1] < 0) << 1 |
		int(dist[0] < 0) << 0;

	float fx = pos % xMax;
	float fy = (pos / xMax) % yMax;
	float fz = pos / (xMax * yMax);

	float offset = 0.1;
	fx*=offset;
	fy*=offset;
	fz*=offset;
	
	vec3 positions[8];
	int edgeIndex = triTable[id + caseCode * 16];
	vec3 vertex;

	if (edgeIndex == 0)
	{
		positions[0] = vec3(fx, fy, fz);
		positions[1] = vec3(fx + offset, fy, fz);
		vertex = interpolation(positions[0], positions[1], dist[0], dist[1]);
	}		
	else if (edgeIndex == 1)
	{
		positions[1] = vec3(fx + offset, fy, fz);
		positions[2] = vec3(fx + offset, fy, fz + offset);
		vertex = interpolation(positions[1], positions[2], dist[1], dist[2]);
	}		
	else if (edgeIndex == 2)
	{
		positions[2] = vec3(fx + offset, fy, fz + offset);
		positions[3] = vec3(fx, fy, fz + offset);
		vertex = interpolation(positions[2], positions[3], dist[2], dist[3]);
	}	
	else if (edgeIndex == 3)
	{
		positions[0] = vec3(fx, fy, fz);
		positions[3] = vec3(fx, fy, fz + offset);
		vertex = interpolation(positions[3], positions[0], dist[3], dist[0]);
	}		
	else if (edgeIndex == 4)
	{
		positions[4] = vec3(fx, fy + offset, fz);
		positions[5] = vec3(fx + offset, fy + offset, fz);
		vertex = interpolation(positions[4], positions[5], dist[4], dist[5]);
	}		
	else if (edgeIndex == 5)
	{
		positions[5] = vec3(fx + offset, fy + offset, fz);
		positions[6] = vec3(fx + offset, fy + offset, fz + offset);
		vertex = interpolation(positions[5], positions[6], dist[5], dist[6]);
	}		
	else if (edgeIndex == 6)
	{
		positions[6] = vec3(fx + offset, fy + offset, fz + offset);
		positions[7] = vec3(fx, fy + offset, fz + offset);
		vertex = interpolation(positions[6], positions[7], dist[6], dist[7]);
	}		
	else if (edgeIndex == 7)
	{
		positions[4] = vec3(fx, fy + offset, fz);
		positions[7] = vec3(fx, fy + offset, fz + offset);
		vertex = interpolation(positions[7], positions[4], dist[7], dist[4]);
	}		
	else if (edgeIndex == 8)
	{
		positions[0] = vec3(fx, fy, fz);
		positions[4] = vec3(fx, fy + offset, fz);
		vertex = interpolation(positions[0], positions[4], dist[0], dist[4]);
	}		
	else if (edgeIndex == 9)
	{
		positions[1] = vec3(fx + offset, fy, fz);
		positions[5] = vec3(fx + offset, fy + offset, fz);
		vertex = interpolation(positions[1], positions[5], dist[1], dist[5]);
	}		
	else if (edgeIndex == 10)
	{
		positions[2] = vec3(fx + offset, fy, fz + offset);
		positions[6] = vec3(fx + offset, fy + offset, fz + offset);
		vertex = interpolation(positions[2], positions[6], dist[2], dist[6]);
	}		
	else if (edgeIndex == 11)
	{
		positions[3] = vec3(fx, fy, fz + offset);
		positions[7] = vec3(fx, fy + offset, fz + offset);
		vertex = interpolation(positions[3], positions[7], dist[3], dist[7]);
	}
	worldPosition = (model * vec4(vertex,1.0)).xyz;
	gl_Position = projection * view * model * vec4(vertex, 1.0);
}