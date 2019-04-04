#version 430
layout(location = 0) in vec3 position;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	uv = position.xy;
	gl_Position = vec4(position, 1.0);
}