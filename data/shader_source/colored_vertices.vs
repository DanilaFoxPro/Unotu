#version 330 core
layout (location = 0) in mediump vec2 position;
layout (location = 1) in lowp    vec4 color;

out lowp vec4 vertex_color;

void main()
{
    gl_Position = vec4( (position.x-0.5f)*2.0f, (position.y-0.5f)*2.0f, 1.0f, 1.0);
    vertex_color = color;
}
