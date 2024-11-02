#version 460 core

layout (location = 0) out vec4 f_fragColor;

in VS_OUT
{
    vec2 texCoord;
} fs_in;

void main()
{
    f_fragColor = vec4(fs_in.texCoord, 1.0, 1.0);
}
