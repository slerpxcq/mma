#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in ivec4 a_bones;
layout (location = 4) in vec3 a_weights;
layout (location = 5) in vec3 a_sdef_c;

out VS_OUT {
	vec2 texCoord;
	vec3 normal;
} vs_out;

uniform mat4 u_view;
uniform mat4 u_proj;

layout (binding = 1, std140) readonly buffer Skinning
{
	mat2x4 data[];
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

bool IsSDEF(in ivec4 bones)
{
	return bones[2] == -1 && bones[3] != -1;
}

mat3 QuatToMat(in vec4 q)
{
	mat3 mat = mat3(1);
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;
	mat[0][0] = w*w + x*x - y*y - z*z; 
	mat[0][1] = 2*w*z + 2*x*y; 
	mat[0][2] = 2*x*z - 2*w*y;
	mat[1][0] = 2*x*y - 2*w*z;
	mat[1][1] = w*w - x*x + y*y - z*z;
	mat[1][2] = 2*y*z + 2*w*x;
	mat[2][0] = 2*x*z + 2*w*y;
	mat[2][1] = 2*y*z - 2*w*x;
	mat[2][2] = w*w - x*x - y*y + z*z;
	return mat;
}

vec4 QuatInv(in vec4 q)
{
	return vec4(q.x, -q.y, -q.z, -q.w) / dot(q,q);
}

vec4 QuatConj(in vec4 q)
{
	return vec4(-q.x, -q.y, -q.x, q.w);
}

mat4 QVPairToMatrix(in mat2x4 pair)
{
	vec4 q = pair[0];
	vec4 t = pair[1];

	mat3 Q = QuatToMat(q);
	mat4 mat = mat4(1);
	mat[0] = vec4(Q[0], 0);
	mat[1] = vec4(Q[1], 0);
	mat[2] = vec4(Q[2], 0);
	mat[3] = t;

	return mat;
}

void Skin(in vec3 inPos, in vec3 inNormal, out vec3 outPos, out vec3 outNormal)
{
	mat4 matSum = mat4(0);
	vec4 weights = vec4(a_weights, 1-dot(a_weights, vec3(1)));

	for (int i = 0; i < 4; i++) {
		if (a_bones[i] < 0) 
			break;
		matSum += weights[i] * QVPairToMatrix(u_skinning.data[a_bones[i]]);
	}

	if (!IsSDEF(a_bones)) { 
		outPos = (matSum*vec4(inPos, 1)).xyz;
		outNormal = (matSum*vec4(inNormal, 0)).xyz;
	} else { 
		vec4 q0 = u_skinning.data[a_bones[0]][0];
		vec4 q1 = u_skinning.data[a_bones[1]][0];
		vec4 q = mix(q0, q1, weights[0]);

		// I have no idea why only this works
		q = vec4(-q.x, -q.y, q.z, q.w);

		q = normalize(q);
		mat3 Q = QuatToMat(q);
		vec3 v0 = Q * (inPos - a_sdef_c);
		vec3 v1 = (matSum*vec4(a_sdef_c, 1)).xyz;

		outPos = v0 + v1;
		outNormal = Q * inNormal;
	}
}

void main()
{
	vec3 skPos, skNormal;
	vec3 morphPos = a_position + u_morph.offsets[gl_VertexID].pos;
	Skin(morphPos, a_normal, skPos, skNormal);

	gl_Position = u_proj * u_view * vec4(skPos, 1);
	vs_out.texCoord = a_texCoord + u_morph.offsets[gl_VertexID].uv;
	vs_out.normal = skNormal;

	u_morph.offsets[gl_VertexID].pos = vec3(0);
	u_morph.offsets[gl_VertexID].uv = vec2(0);
};