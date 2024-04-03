#version 460 core

out vec4 f_fragColor;

uniform vec4 u_color;

void main()
{
	f_fragColor = u_color;
}
