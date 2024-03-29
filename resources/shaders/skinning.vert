#version 430 core

// ********************* Attributes *********************
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in ivec4 bones;
layout (location = 4) in vec3 weights;

vec4 w = vec4(weights, 1-dot(vec3(1), weights));
// ********************* END Attributes *********************

// ********************* BEGIN uniforms *********************
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightDir;
uniform vec3 lightColor;

uniform sampler2D toon;
uniform sampler2D sph;

layout (binding = 0, std140) uniform Material
{
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	vec4 edge;
	float edgeSize;
	int  flags;
} material;

layout (binding = 1, std140) readonly buffer Skinning
{
	mat4 skinning[];
};

layout (binding = 2, std140) buffer Offset
{
	struct {
		vec3  pos;
		float _pad0;
		vec2  uv;
		vec2  _pad1;
	} offsets[];
};

// ********************* END uniforms *********************

out VS_OUT 
{
	vec2 uv;	
	vec4 color;
} vs_out;

void Skin(in vec3 inPos, in vec3 inNormal, out vec3 outPos, out vec3 outNormal)
{
	mat4 matSum = mat4(0);

	for (int i = 0; i < 4; i++) {
		if (bones[i] > 0) 
			matSum += w[i] * skinning[bones[i]];
	}

	outPos = (matSum*vec4(inPos, 1)).xyz;
	outNormal = (inverse(transpose(matSum))*vec4(inNormal, 0)).xyz;
}

void main()
{
	// Morph
	vec3 morphPosition = position + offsets[gl_VertexID].pos;

	// Skinning
	vec3 skinNormal, skinPos;
	Skin(morphPosition, normal, skinPos, skinNormal);

	// Lighting
	vec3 N = skinNormal;
	vec3 L = -normalize(lightDir);
	vec3 V = view[3].xyz;
	vec3 H = normalize(L + V);
	float NoL = dot(N, L);
	float diffuseWeight = max(NoL, 0);
	float specularWeight = pow(max(dot(N, H), 0), material.specular.a);

	// Toon
	vec3 toonColor = texture(toon, vec2(0.5, NoL*0.5 + 0.5)).rgb;

	// Sphere
	vec3 viewN = mat3(view)*skinNormal;
	vec2 sphUv = viewN.xy*0.5 + 0.5;
	int sphMode = (material.flags >> 8) & 3;
	vec3 sphColor = texture(sph, sphUv).rgb;
	vec3 sphAdd = (sphMode == 2) ? sphColor : vec3(0);
	vec3 sphMul = (sphMode == 1) ? sphColor : vec3(1);

	vec3 direct = material.diffuse.rgb * lightColor * diffuseWeight
		+ material.specular.rgb * lightColor * specularWeight;

	vs_out.color.rgb = sphMul * ((material.ambient.rgb ) + direct * toonColor)+ sphAdd;
	vs_out.color.a = material.diffuse.a;

	// Output
	gl_Position = proj * view * vec4(skinPos, 1.f);
	vs_out.uv = uv + offsets[gl_VertexID].uv;

	// Clear morph
	offsets[gl_VertexID].uv = vec2(0);
	offsets[gl_VertexID].pos = vec3(0);
}

