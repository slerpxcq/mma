#version 460 core

layout (location = 0) out vec4 f_fragColor;

in VS_OUT
{
	vec3 texCoord;
} fs_in;

uniform samplerCube u_skybox;

void main()
{
	f_fragColor = texture(u_skybox, fs_in.texCoord);
}