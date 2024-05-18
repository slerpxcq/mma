#version 430 core

layout (location = 0) out vec4 f_fragColor;

uniform sampler2D u_albedo;
uniform sampler2D u_sph;
uniform sampler2D u_toon;
uniform samplerCube u_skybox;

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
	vec4 texTint;
	vec4 toonTint;
	vec4 sphTint;
	float edgeSize;
	uint  flags;
} u_material;

layout (binding = 1, std140) uniform Camera 
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
} u_camera;

layout (binding = 2, std140) uniform Light 
{
	vec3 color;
	vec3 direction;
} u_light;

void main()
{
	vec3 N = fs_in.normal;
	vec3 L = -normalize(u_light.direction);
	vec3 V = normalize(vec3(u_camera.view[3]) - fs_in.position);
	vec3 H = normalize(L+V);
	vec3 I = -V;
	vec3 R = reflect(I, N);
	R.z = -R.z;

	vec4 color = vec4(0);
	/* Ambient */
	color.rgb += u_material.ambient.rgb * u_light.color * 2;
	/* Diffuse */
	//color.rgb += max(0, dot(fs_in.normal, L)) * u_material.diffuse.rgb;
	color.a = u_material.diffuse.a;
	color = clamp(color, 0.0, 1.0);

	/* Albedo */
	color *= texture(u_albedo, fs_in.texCoord) * u_material.texTint;

	/* Sphere */
	vec3 vN = mat3(u_camera.view) * N;
	vec2 sphTexCoord = vN.xy*0.5 + 0.5;
	uint sphMode = (u_material.flags >> 8) & 3;
	vec3 sphColor = texture(u_sph, sphTexCoord).rgb * vec3(u_material.sphTint);
	if (sphMode == 1) {
		color.rgb *= sphColor;
	} else if (sphMode == 2) {
		color.rgb += sphColor;
	}

	/* Toon */
	float toonFactor = max(0, dot(fs_in.normal, L));
	color.rgb *= texture(u_toon, vec2(0, 0.5 - toonFactor * 0.5)).rgb * vec3(u_material.toonTint);

	/* Specular */
	vec3 specular = pow(max(0, dot(N,H)), u_material.specular.w) * u_material.specular.rgb * u_light.color;
	color.rgb += specular;

	f_fragColor = color;
}