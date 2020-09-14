#version 330 core
in lowp vec4 vertex_color;

out lowp vec4 color;

void main()
{
	color = vertex_color;
}
