#version 430

out vec4 fragColor;


in vertex_data
{
	vec2 vertex_uv;
};

uniform sampler2D texture;

void main()
{
	fragColor = vec4(0,0,0,0);

	if(vertex_uv.x - 0.01 < 0 || vertex_uv.x + 0.01 > 1.0)
		fragColor = vec4(0.8,0.8,0.0,1);
	if(vertex_uv.y - 0.01 < 0 || vertex_uv.y + 0.01 > 1.0)
		fragColor = vec4(0.8,0.8,0.0,1);
}