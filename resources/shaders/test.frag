#version 460 core

layout (location = 0) out vec4 f_fragColor;

uniform sampler2D u_albedo;

in VS_OUT
{
    vec2 texCoord;
} fs_in;

void main()
{
    f_fragColor = texture(u_albedo, fs_in.texCoord);
    // f_fragColor = vec4(fs_in.texCoord, 1, 1);
}
