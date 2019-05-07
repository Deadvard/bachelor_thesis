#version 430

struct DrawArraysIndirectCommand
{
	uint count;
	uint primCount;
	uint first;
	uint baseInstance;
};

layout(std430, binding = 4) buffer inBuffer
{
	int distances[];
};

layout(std430, packed, binding = 5) buffer histoPyramid
{
	DrawArraysIndirectCommand vertices;	
	int verticesPerCell[];
};

void main()
{
	
}