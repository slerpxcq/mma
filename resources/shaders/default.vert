#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec2 a_texCoord1;
layout (location = 4) in vec2 a_texCoord2;
layout (location = 5) in vec2 a_texCoord3;
layout (location = 6) in vec2 a_texCoord4;

layout (location = 7) in ivec4 a_bones;
layout (location = 8) in vec3 a_weights;

layout (location = 9) in vec3 a_sdef_c;
layout (location = 10) in vec3 a_sdef_r0;
layout (location = 11) in vec3 a_sdef_r1;

layout (std140, binding = 0) 
uniform MM_GlobalUniform
{
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
} g;

out VS_OUT 
{
    vec2 texCoord;
} vs_out;

void main() 
{
    gl_Position = g.viewProjection * vec4(a_position, 1.0);
    vs_out.texCoord = a_texCoord;
}
