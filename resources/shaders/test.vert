#version 430 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;

out VS_OUT {
	vec4 color;
	vec2 texCoord;
} vs_out;

uniform vec4 c;

void main()
{
	gl_Position = vec4(pos, 0, 1);
	vs_out.color = c;
	vs_out.texCoord = 0.5 * (pos + vec2(1));
}
