#version 430
layout(location = 0) in vec3 position;

uniform mat4 model;

layout(std140, binding = 0) uniform matrices
{
	uniform mat4 view;
	uniform mat4 projection;
};

void main()
{
	gl_Position = projection * view * vec4(position, 1.0);
}