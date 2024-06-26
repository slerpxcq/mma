#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in ivec4 a_bones;
layout (location = 4) in vec3 a_weights;
layout (location = 5) in vec3 a_sdef_c;
layout (location = 6) in vec3 a_sdef_r0;
layout (location = 7) in vec3 a_sdef_r1;

out VS_OUT {
	vec2 texCoord;
	vec3 position;
	vec3 normal;
} vs_out;

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

layout (binding = 1, std140) readonly buffer Skinning
{
	mat2x4 data[];
} u_skinning;

layout (binding = 2, std140) buffer Morph
{
	struct {
		vec3  pos;
		vec2  uv;
	} offsets[];
} u_morph;

uniform bool u_outlinePass;
uniform bool u_resetMorph;

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
	mat[0][0] = 1-2*y*y-2*z*z;
	mat[0][1] = 2*x*y+2*w*z;
	mat[0][2] = 2*x*z-2*w*y;
	mat[1][0] = 2*x*y-2*w*z;
	mat[1][1] = 1-2*x*x-2*z*z;
	mat[1][2] = 2*y*z+2*w*x;
	mat[2][0] = 2*x*z+2*w*y;
	mat[2][1] = 2*y*z-2*w*x;
	mat[2][2] = 1-2*x*x-2*y*y;
	return mat;
}

mat4 QVPairToMatrix(vec4 q, vec3 t)
{
	mat3 Q = QuatToMat(q);
	mat4 mat = mat4(1);
	mat[0] = vec4(Q[0], 0);
	mat[1] = vec4(Q[1], 0);
	mat[2] = vec4(Q[2], 0);
	mat[3] = vec4(t, 1);

	return mat;
}

void Skin(in vec3 inPos, in vec3 inNormal, out vec3 outPos, out vec3 outNormal)
{
	mat4 matSum = mat4(0);
	vec4 weights = vec4(a_weights, 1-dot(a_weights, vec3(1)));

	for (int i = 0; i < 4; i++) {
		if (a_bones[i] < 0) 
			break;
		vec4 q = u_skinning.data[a_bones[i]][0];
		vec3 t = vec3(u_skinning.data[a_bones[i]][1]);
		matSum += weights[i] * QVPairToMatrix(q, t);
	}

	if (!IsSDEF(a_bones)) { 
		outPos = vec3(matSum * vec4(inPos, 1));
		outNormal = inverse(transpose(mat3(matSum))) * inNormal;
	} else { 
		vec4 q0 = u_skinning.data[a_bones[0]][0];
		vec4 q1 = u_skinning.data[a_bones[1]][0];
		if (dot(q0, q1) < 0) 
			q1 = -q1;
		vec4 q = normalize(mix(q1, q0, weights[0]));
		mat3 Q = QuatToMat(q);
		vec3 v0 = Q * (inPos - a_sdef_c);
		vec3 v1 = vec3(matSum*vec4(a_sdef_c, 1));

		outPos = v0 + v1;
		outNormal = Q * inNormal;
	}
}

void main()
{
	vec3 skPos, skNormal;
	vec3 morphPos = a_position + u_morph.offsets[gl_VertexID].pos;
	Skin(morphPos, a_normal, skPos, skNormal);

	if (u_outlinePass) {
		gl_Position = u_camera.viewProj * vec4(skPos + 0.01 * skNormal, 1);
	} else {
		gl_Position = u_camera.viewProj * vec4(skPos, 1);
	}

	vs_out.position = skPos;
	vs_out.normal = skNormal;
	vs_out.texCoord = a_texCoord + u_morph.offsets[gl_VertexID].uv;

	if (!u_outlinePass) {
		u_morph.offsets[gl_VertexID].pos = vec3(0);
		u_morph.offsets[gl_VertexID].uv = vec2(0);
	}
};