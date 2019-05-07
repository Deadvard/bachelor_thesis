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

	uint startingOffset = 349520;

	int temp = verticesPerCell[id];

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(id > temp)
			{
				id -= temp;
			}
			else
			{
				break;
			}
		}
	}

}