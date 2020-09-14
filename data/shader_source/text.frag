#version 330 core
out vec4 out_color;

in vec2 texture_coordinates;

uniform sampler2D fontTexture;
uniform lowp vec4 fontColor;
uniform lowp vec4 fontBackgroundColor;

void main()
{
	lowp vec4 mask = texture( fontTexture, texture_coordinates );
	// I like this so much! (vec4.rgb)
	out_color = mix( fontBackgroundColor, fontColor, mask.r );
	// out_color = vec4( fontColor.rgb, fontColor.a*mask.r );
}
