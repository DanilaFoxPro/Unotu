#version 330 core
out lowp vec4 out_color;

in mediump vec2 texture_coordinates;

uniform sampler2D daTexture;

void main()
{
	out_color = texture( daTexture, texture_coordinates );
	//out_color = vec4( texture_coordinates.x, texture_coordinates.y, 0.0, 1.0 );
	//out_color = texture( daTexture, vec2(0.0, 0.0) );
	//out_color = vec4( texture_coordinates.x, texture_coordinates.y, 0.0, 1.0 );
}
