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
vec3 getNormalFromMap(vec2 texcoord0,vec3 worldPos,vec3 normal)
{
    vec3 tangentNormal = vec3(0.0, 0.0, 0.0);
	tangentNormal.xy = texture2DBc5(s_normalMap, texcoord0) * 2.0 - 1.0;
	tangentNormal.z  = sqrt(1.0 - dot(tangentNormal.xy, tangentNormal.xy) );

    vec3 Q1  = dFdx(worldPos);
    vec3 Q2  = dFdy(worldPos);
    vec2 st1 = dFdx(texcoord0);
    vec2 st2 = dFdy(texcoord0);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.y - Q2*st1.y); // Q1*st2.t - Q2*st1.t
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

////////////////////////////////////////////////////////////




vec3 calcFresnel(vec3 _cspec, float _dot, float _strength)
{
	return _cspec + (1.0 - _cspec)*pow(1.0 - _dot, 5.0) * _strength;
}

vec3 calcLambert(vec3 _cdiff, float _ndotl)
{
	return _cdiff*_ndotl;
}

vec3 calcBlinn(vec3 _cspec, float _ndoth, float _ndotl, float _specPwr)
{
	float norm = (_specPwr+8.0)*0.125;
	float brdf = pow(_ndoth, _specPwr)*_ndotl*norm;
	return _cspec*brdf;
}

float specPwr(float _gloss)
{
	return exp2(10.0*_gloss+2.0);
}

void main()
{
    vec3 albedo = pow(texture2D(albedoMap, v_texcoord0).rgb, vec3(2.2));



	vec3 color = vec3(0.0, 0.0, 0.0);
	gl_FragColor.xyz = color;
	gl_FragColor.w = 1.0;
}
