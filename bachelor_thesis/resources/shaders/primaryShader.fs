#version 430

in vec2 uv;
out vec4 fragColor;

uniform sampler2D texture;

void main()
{
	fragColor = texture2D(texture, uv);
}