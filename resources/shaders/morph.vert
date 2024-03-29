#version 430 core

layout (location = 0) in vec3 posOffset;
layout (location = 1) in vec2 uvOffset;
layout (location = 2) in int vertexIndex;

uniform float u_weight;

layout (binding = 2, std140) buffer Offset
{
	struct {
		vec3  pos;
		float pad0;
		vec2  uv;
		vec2  pad1;
	} offsets[];
};

void main()
{
	offsets[vertexIndex].pos += posOffset * u_weight ;
	offsets[vertexIndex].uv += uvOffset * u_weight ;
}