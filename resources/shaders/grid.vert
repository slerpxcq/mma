#version 460 core

layout (location = 0) in vec3 a_position;

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

void main()
{
	gl_Position = u_camera.viewProj * vec4(a_position, 1);
}