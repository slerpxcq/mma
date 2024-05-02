
#version 430 core

layout (location = 0) out vec4 f_fragColor;

// https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/6.pbr/1.1.lighting/1.1.pbr.fs

uniform sampler2D u_albedo;
uniform sampler2D u_sph;
uniform sampler2D u_toon;
uniform samplerCube u_skybox;

// material parameters
//uniform vec3 albedo;
uniform float u_metallic;
uniform float u_roughness;
//uniform float ao;
//
//// lights
//uniform vec3 lightPositions[4];
//uniform vec3 lightColors[4];
//
//uniform vec3 camPos;

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

//layout (binding = 0, std140) uniform PBRMaterial 
//{
//    vec4 albedo;
//    float metallic;
//    float roughness;
//    float ao;
//} u_pbrMaterial;

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

//void main()
//{
//	vec3 N = fs_in.normal;
//	vec3 L = -normalize(u_light.direction);
//	vec3 V = normalize(vec3(u_camera.view[3]) - fs_in.position);
//	vec3 H = normalize(L+V);
//	vec3 I = -V;
//	vec3 R = reflect(I, N);
//	R.z = -R.z;
//
//	f_fragColor = color;
//}
const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{		
//    vec3 N = normalize(Normal);
	vec3 N = fs_in.normal;
	vec3 L = -normalize(u_light.direction);
	vec3 V = normalize(vec3(u_camera.view[3]) - fs_in.position);
	vec3 H = normalize(L+V);
	vec3 I = -V;
	vec3 R = reflect(I, N);
	R.z = -R.z;
//	vec3 N = fs_in.normal;
//    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    vec4 albedoTexColor = texture(u_albedo, fs_in.texCoord);
    vec3 albedo = pow(albedoTexColor.rgb, vec3(2.2));

    F0 = mix(F0, albedo, u_metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    //for(int i = 0; i < 4; ++i) 
    //{
        // calculate per-light radiance
        //vec3 L = normalize(lightPositions[i] - WorldPos);
        //vec3 H = normalize(V + L);
        //float distance = length(lightPositions[i] - WorldPos);
        //float attenuation = 1.0 / (distance * distance);
        //vec3 radiance = lightColors[i] * attenuation;
        vec3 radiance = u_light.color;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, u_roughness);   
        float G   = GeometrySmith(N, V, L, u_roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - u_metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    //}   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 ambient = vec3(0.03) * albedo;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    f_fragColor = vec4(color, u_material.diffuse.a * albedoTexColor.a);
}