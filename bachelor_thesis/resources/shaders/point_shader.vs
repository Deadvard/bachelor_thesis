#version 430
layout(location = 0) in vec3 position;

void main()
{
	gl_Position = gl_Position = projection * view * vec4(position, 1);
}