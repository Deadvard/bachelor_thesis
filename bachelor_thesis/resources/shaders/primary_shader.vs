#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 offset;

out vertex_data
{
	vec2 vertex_uv;
};

uniform mat4 model;

layout(std140, binding = 0) uniform matrices
{
	uniform mat4 view;
	uniform mat4 projection;
};

void main()
{
	vertex_uv = uv;
	gl_Position = projection * view * vec4(position + offset, 1.0);
}