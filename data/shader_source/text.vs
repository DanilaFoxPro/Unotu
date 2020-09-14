#version 330 core
layout (location = 0) in mediump vec2 position;
layout (location = 1) in mediump vec2 texture_coordinates_in;

out mediump vec2 texture_coordinates;

void main()
{
    gl_Position = vec4( (position.x-0.5f)*2.0f, (position.y-0.5f)*2.0f, 1.0f, 1.0);
    texture_coordinates = vec2( texture_coordinates_in.x, texture_coordinates_in.y);
}
