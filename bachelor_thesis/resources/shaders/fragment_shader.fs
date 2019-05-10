#version 430

in vec3 faceNormal;
out vec4 fragColor;

void main()
{
	fragColor = vec4(faceNormal, 1.0);
}