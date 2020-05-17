$input v_view, v_normal, v_worldPos, v_texcoord0

#include "../common.sh"
#include "uniforms.sh"

// material parameters


SAMPLERCUBE(s_texCube, 0);
SAMPLERCUBE(s_texCubeIrr, 1);
SAMPLER2D(s_brdfLUT,2);

SAMPLER2D(s_albedoMap,3);
SAMPLER2D(s_normalMap,4);
SAMPLER2D(s_metallicMap,5);
SAMPLER2D(s_roughnessMap,6);
SAMPLER2D(s_aoMap,7);

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(vec3 worldPos,vec3 normal,vec2 texcoord0)
{
    vec3 tangentNormal = mul(texture2D(s_normalMap, texcoord0).xyz ,2.0) - 1.0;

    vec3 Q1  = dFdx(worldPos);
    vec3 Q2  = dFdy(worldPos);
    vec2 st1 = dFdx(texcoord0);
    vec2 st2 = dFdy(texcoord0);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(mul(Q1, st2.y) - mul(Q2, st1.y)); // Q1*st2.t - Q2*st1.t
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(mul(TBN, tangentNormal) );
}
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
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness,1.0 - roughness,1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

void main()
{
    vec3 albedo = pow(texture2D(s_albedoMap, v_texcoord0).rgb, vec3(2.2, 2.2, 2.2));
	vec3 metallics = texture2D(s_metallicMap, v_texcoord0).rgb;
	float metallic = texture2D(s_metallicMap, v_texcoord0).r;
    float roughness = texture2D(s_roughnessMap, v_texcoord0).r;
    float ao = texture2D(s_aoMap, v_texcoord0).r;

	vec3 N = v_normal /*getNormalFromMap(v_worldPos, v_normal, v_texcoord0)*/;  //Use the model with normal for now
    vec3 V = normalize(u_camPos - v_worldPos);
    vec3 R = reflect(-V, N); 

	vec3 F0 = vec3(0.04, 0.04, 0.04); 
    F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0, 0.0, 0.0);

	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD = mul(kD, 1.0 - metallic);

	vec3 irradiance = textureCube(s_texCubeIrr, N).rgb;
    vec3 diffuse      = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = toFilmic(toLinear(textureCubeLod(s_texCube, R,  roughness * MAX_REFLECTION_LOD)));    
	vec2 brdf  = texture2D(s_brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = mul(prefilteredColor, F/*mul(F , brdf.x) + brdf.y*/);//Temporarily remove BRDF effects

	vec3 ambient = mul(mul(kD, diffuse) + specular, ao);
    
    vec3 color = ambient + Lo;
	// HDR tonemapping
    color = color / (color + vec3(1.0,1.0,1.0));
    // gamma correct
    color = pow(color, vec3_splat(1.0/2.2)); 

	gl_FragColor.xyz = color;
	gl_FragColor.w = 1.0;
}
