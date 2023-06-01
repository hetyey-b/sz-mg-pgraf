#version 130

uniform mat4 MVP;
uniform vec3 points[10];

void main()
{
	gl_Position = MVP * vec4(points[gl_VertexID], 1) ;
}