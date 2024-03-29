#version 430 core

in VS_OUT 
{
	vec2 uv;
	vec4 color;
} fs_in;

out vec4 f_fragColor;

uniform sampler2D tex;

void main()
{
	//fragColor = fs_in.color * texture(tex, fs_in.uv);
	f_fragColor = vec4(1, 0, 0, 1);
}

