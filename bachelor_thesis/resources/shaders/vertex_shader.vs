#version 430

layout(std430, binding = 4) buffer inBuffer
{
	int distances[];
};

layout(std430, packed, binding = 5) buffer histoPyramid
{
	int verticesPerCell[];
};

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



}