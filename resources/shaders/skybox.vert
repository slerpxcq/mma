#version 460 core

out VS_OUT
{
	vec3 texCoord;
} vs_out;

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

void main()
{
    const vec3 vertices[] = vec3[36](
		vec3(-1.0f,  1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3( 1.0f, -1.0f, -1.0f),
		vec3( 1.0f, -1.0f, -1.0f),
		vec3( 1.0f,  1.0f, -1.0f),
		vec3(-1.0f,  1.0f, -1.0f),
								 
		vec3(-1.0f, -1.0f,  1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f,  1.0f, -1.0f),
		vec3(-1.0f,  1.0f, -1.0f),
		vec3(-1.0f,  1.0f,  1.0f),
		vec3(-1.0f, -1.0f,  1.0f),
								 
		vec3( 1.0f, -1.0f, -1.0f),
		vec3( 1.0f, -1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3( 1.0f,  1.0f, -1.0f),
		vec3( 1.0f, -1.0f, -1.0f),
								 
		vec3(-1.0f, -1.0f,  1.0f),
		vec3(-1.0f,  1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3( 1.0f, -1.0f,  1.0f),
		vec3(-1.0f, -1.0f,  1.0f),
								 
		vec3(-1.0f,  1.0f, -1.0f),
		vec3( 1.0f,  1.0f, -1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3( 1.0f,  1.0f,  1.0f),
		vec3(-1.0f,  1.0f,  1.0f),
		vec3(-1.0f,  1.0f, -1.0f),
								 
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f,  1.0f),
		vec3( 1.0f, -1.0f, -1.0f),
		vec3( 1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f,  1.0f),
		vec3( 1.0f, -1.0f,  1.0f));

	vec3 position = vertices[gl_VertexID];
	gl_Position = u_camera.proj * mat4(mat3(u_camera.view)) * vec4(position, 1.0);
	vs_out.texCoord = position;
}