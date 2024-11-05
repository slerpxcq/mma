#version 460 core

layout (location = 0) out vec4 f_fragColor;

in vec4 vert_color;

void main()
{
    f_fragColor = vert_color;
}