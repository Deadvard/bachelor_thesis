#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(std140, binding = 0) uniform matrices
{
	uniform mat4 view;
	uniform mat4 projection;
};

out vec3 col;

void main()
{
	col = color;
	gl_Position = projection * view * vec4(position, 1);
}