#version 430 core

layout (location = 0) out vec4 f_fragColor;

layout (binding = 0, std140) uniform Material 
{
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	vec4 edge;
	float edgeSize;
	uint  flags;
} u_material;

void main()
{
    f_fragColor = u_material.edge;
}