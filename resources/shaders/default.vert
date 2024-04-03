#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in ivec4 a_bones;
layout (location = 4) in vec3 a_weights;

uniform mat4 u_view;
uniform mat4 u_proj;

layout (binding = 1, std140) readonly buffer Skinning
{
	mat4 matrices[];
} u_skinning;

layout (binding = 2, std140) buffer Morph
{
	struct {
		vec3  pos;
		float pad0;
		vec2  uv;
		vec2 pad1;
	} offsets[];
} u_morph;

out VS_OUT {
	vec2 texCoord;
	vec3 normal;
} vs_out;

void Skin(in vec3 inPos, in vec3 inNormal, out vec3 outPos, out vec3 outNormal)
{
	mat4 matSum = mat4(0);
	vec4 weights = vec4(a_weights, 1-dot(a_weights, vec3(1)));

	for (int i = 0; i < 4; i++) {
		if (a_bones[i] > 0) 
			matSum += weights[i] * u_skinning.matrices[a_bones[i]];
	}

	outPos = (matSum*vec4(inPos, 1)).xyz;
	outNormal = (inverse(transpose(matSum))*vec4(inNormal, 0)).xyz;
}

void main()
{
	vec3 skPos, skNormal;
	Skin(a_position + u_morph.offsets[gl_VertexID].pos, a_normal, skPos, skNormal);

	gl_Position = u_proj * u_view * vec4(skPos, 1);
	vs_out.texCoord = a_texCoord + u_morph.offsets[gl_VertexID].uv;
	vs_out.normal = skNormal;

	u_morph.offsets[gl_VertexID].pos = vec3(0);
	u_morph.offsets[gl_VertexID].uv = vec2(0);
};