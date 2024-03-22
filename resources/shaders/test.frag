#version 430 core

in VS_OUT {
	vec4 color;
	vec2 texCoord;
} fs_in;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	//fragColor = fs_in.color;
	//fragColor = vec4(1,0,0,1);
	fragColor = fs_in.color * texture(tex, fs_in.texCoord);
}