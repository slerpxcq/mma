#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 f_fragColor;

layout (std140, binding = 0) 
uniform MM_GlobalUniform
{
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
} g;

in VS_OUT
{
    vec2 texCoord;
} fs_in;

void main()
{
    f_fragColor = vec4(fs_in.texCoord, 1.0, 1.0);
}
