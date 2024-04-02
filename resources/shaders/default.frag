#version 430 core

out vec4 f_fragColor;

uniform sampler2D u_albedo;

in vec2 texCoord;

in VS_OUT {
	vec2 texCoord;
} fs_in;

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
	f_fragColor = u_material.diffuse * texture(u_albedo, fs_in.texCoord);
}