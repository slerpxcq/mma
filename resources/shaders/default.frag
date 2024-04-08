#version 430 core

layout (location = 0) out vec4 f_fragColor;

uniform sampler2D u_albedo;
uniform sampler2D u_sph;
uniform sampler2D u_toon;
uniform samplerCube u_skybox;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

in VS_OUT {
	vec2 texCoord;
	vec3 position;
	vec3 normal;
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

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

uniform bool u_outlinePass;

void main()
{
	if (u_outlinePass) {
		f_fragColor = u_material.edge;
		return;
	}

	vec3 N = fs_in.normal;
	vec3 L = -normalize(u_lightDir);
	vec3 V = normalize(vec3(u_camera.view[3]) - fs_in.position);
	vec3 H = normalize(L+V);
	vec3 I = -V;
	vec3 R = reflect(I, N);
	R.z = -R.z;

	vec2 toonUV = vec2(0, max(0,dot(N,L)));

	vec4 color = vec4(0);
	color.rgb = u_lightColor * texture(u_toon, toonUV).rgb * u_material.diffuse.rgb + u_material.ambient.rgb*0.1;
	color.a = u_material.diffuse.a;
	vec3 specular = pow(max(0, dot(N,H)), u_material.specular.w) * u_material.specular.rgb * u_lightColor;
	color *= texture(u_albedo, fs_in.texCoord);

	// Sphere
	vec3 vN = mat3(u_camera.view) * N;
	vec2 sphTexCoord = vN.xy*0.5 + 0.5;
	uint sphMode = (u_material.flags >> 8) & 3;
	vec3 sphColor = texture(u_sph, sphTexCoord).rgb;
	vec3 sphMul = (sphMode == 1) ? sphColor : vec3(1);
	vec3 sphAdd = (sphMode == 2) ? sphColor : vec3(0);

	color.rgb = sphMul * color.rgb + sphAdd;
	color.rgb += specular;

	//f_fragColor = color * texture(u_skybox, R);
	//f_fragColor = texture(u_skybox, R);
	f_fragColor = color;
}