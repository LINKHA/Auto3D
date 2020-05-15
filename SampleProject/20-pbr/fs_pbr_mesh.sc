$input v_view, v_normal

#include "../common.sh"
#include "uniforms.sh"

// material parameters
SAMPLER2D(s_albedoMap)
SAMPLER2D(s_normalMap)
SAMPLER2D(s_metallicMap)
SAMPLER2D(s_roughnessMap)
SAMPLER2D(s_aoMap)

SAMPLERCUBE(s_texCube, 0);
SAMPLERCUBE(s_texCubeIrr, 1);
SAMPLER2D(s_brdfLUT)


uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
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
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
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
	// Light.
	vec3 ld     = normalize(u_lightDir);
	vec3 clight = u_lightCol;

	// Input.
	vec3 nn = normalize(v_normal);
	vec3 vv = normalize(v_view);
	vec3 hh = normalize(vv + ld);

	float ndotv = clamp(dot(nn, vv), 0.0, 1.0);
	float ndotl = clamp(dot(nn, ld), 0.0, 1.0);
	float ndoth = clamp(dot(nn, hh), 0.0, 1.0);
	float hdotv = clamp(dot(hh, vv), 0.0, 1.0);

	// Material params.
	vec3  inAlbedo       = u_rgbDiff.xyz;
	float inReflectivity = u_reflectivity;
	float inGloss        = u_glossiness;

	// Reflection.
	vec3 refl;
	if (0.0 == u_metalOrSpec) // Metalness workflow.
	{
		refl = mix(vec3_splat(0.04), inAlbedo, inReflectivity);
	}
	else // Specular workflow.
	{
		refl = u_rgbSpec.xyz * vec3_splat(inReflectivity);
	}
	vec3 albedo = inAlbedo * (1.0 - inReflectivity);
	vec3 dirFresnel = calcFresnel(refl, hdotv, inGloss);
	vec3 envFresnel = calcFresnel(refl, ndotv, inGloss);

	vec3 lambert = u_doDiffuse  * calcLambert(albedo * (1.0 - dirFresnel), ndotl);
	vec3 blinn   = u_doSpecular * calcBlinn(dirFresnel, ndoth, ndotl, specPwr(inGloss));
	vec3 direct  = (lambert + blinn)*clight;

	// Note: Environment textures are filtered with cmft: https://github.com/dariomanesku/cmft
	// Params used:
	// --excludeBase true //!< First level mip is not filtered.
	// --mipCount 7       //!< 7 mip levels are used in total, [256x256 .. 4x4]. Lower res mip maps should be avoided.
	// --glossScale 10    //!< Spec power scale. See: specPwr().
	// --glossBias 2      //!< Spec power bias. See: specPwr().
	// --edgeFixup warp   //!< This must be used on DirectX9. When fileted with 'warp', fixCubeLookup() should be used.
	float mip = 1.0 + 5.0*(1.0 - inGloss); // Use mip levels [1..6] for radiance.

	mat4 mtx;
	mtx[0] = u_mtx0;
	mtx[1] = u_mtx1;
	mtx[2] = u_mtx2;
	mtx[3] = u_mtx3;
	vec3 vr = 2.0*ndotv*nn - vv; // Same as: -reflect(vv, nn);
	vec3 cubeR = normalize(instMul(mtx, vec4(vr, 0.0)).xyz);
	vec3 cubeN = normalize(instMul(mtx, vec4(nn, 0.0)).xyz);
	cubeR = fixCubeLookup(cubeR, mip, 256.0);

	vec3 radiance    = toLinear(textureCubeLod(s_texCube, cubeR, mip).xyz);
	vec3 irradiance  = toLinear(textureCube(s_texCubeIrr, cubeN).xyz);
	vec3 envDiffuse  = albedo     * irradiance * u_doDiffuseIbl;
	vec3 envSpecular = envFresnel * radiance   * u_doSpecularIbl;
	vec3 indirect    = envDiffuse + envSpecular;

	// Color.
	vec3 color = direct + indirect;
	color = color * exp2(u_exposure);
	gl_FragColor.xyz = toFilmic(color);
	gl_FragColor.w = 1.0;
}
