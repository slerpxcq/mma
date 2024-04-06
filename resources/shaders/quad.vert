#version 460 core

out VS_OUT
{
	vec2 texCoord;
} vs_out;

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

void main()
{
	const vec2 positions[] = vec2[6](
		vec2(-1, -1), vec2(1, -1), vec2(1, 1),
		vec2(-1, -1), vec2(1, 1), vec2(-1, 1)
	);

	vec2 pos = positions[gl_VertexID];
	gl_Position = vec4(pos, 0, 1);
	vs_out.texCoord = (vec2(1) + pos) * 0.5;
}