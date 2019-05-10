#version 430

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

in vData
{
    vec3 worldPosition;
}vertices[];

out vec3 faceNormal;

void main()
{
   vec3 a = vec3(vertices[0].worldPosition) - vec3(vertices[1].worldPosition);
   vec3 b = vec3(vertices[2].worldPosition) - vec3(vertices[1].worldPosition);
   faceNormal = normalize(cross(a, b));
   
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();
   gl_Position = gl_in[2].gl_Position;
   EmitVertex();
   EndPrimitive();
}