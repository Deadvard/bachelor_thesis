#version 430

out vec4 fragColor;

in vec3 col;

void main()
{
	fragColor = vec4(col, 0.2);
}