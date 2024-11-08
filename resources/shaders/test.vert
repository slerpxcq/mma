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

// mat4 view;
// mat4 projection;
uniform mat4 u_viewProjection;

layout (binding = 0, std140) readonly buffer Skinning
{
    mat4 data[];
} u_skinning;

out VS_OUT 
{
    vec2 texCoord;
} vs_out;

void DoSkinning(in vec3 inPos, in vec3 inNormal, out vec3 outPos, out vec3 outNormal)
{
    mat4 sum = mat4(0);
    vec4 weights = vec4(a_weights, 1-dot(a_weights,vec3(1)));
    for (int i = 0; i < 4; ++i) {
        sum += weights[i] * u_skinning.data[a_bones[i]];
    }
    outPos = vec3(sum * vec4(inPos, 1));
    outNormal = mat3(sum) * inNormal;
}

void main() 
{
    // gl_Position = u_viewProjection * vec4(a_position, 1.0);
    vec3 pos, normal;
    DoSkinning(a_position, a_normal, pos, normal);
    gl_Position = u_viewProjection * vec4(pos, 1);
    vs_out.texCoord = a_texCoord;
}
