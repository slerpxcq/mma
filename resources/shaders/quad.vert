#version 460 core

out VS_OUT
{
	vec2 texCoord;
} vs_out;

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