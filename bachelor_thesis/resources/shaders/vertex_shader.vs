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
	
}