#include "common.sh"

uniform vec4 u_params0;
uniform vec4 u_params1;
uniform vec4 u_params2;
uniform vec4 u_color;

uniform vec4 u_materialKa;
uniform vec4 u_materialKd;
uniform vec4 u_materialKs;
uniform vec4 u_lightPosition;
uniform vec4 u_lightAmbientPower;
uniform vec4 u_lightDiffusePower;
uniform vec4 u_lightSpecularPower;
uniform vec4 u_lightSpotDirectionInner;
uniform vec4 u_lightAttenuationSpotOuter;
uniform vec4 u_smSamplingParams;
uniform vec4 u_csmFarDistances;

#if SM_OMNI
uniform vec4 u_tetraNormalGreen;
uniform vec4 u_tetraNormalYellow;
uniform vec4 u_tetraNormalBlue;
uniform vec4 u_tetraNormalRed;
#endif

SAMPLER2D(s_shadowMap0, 4);
SAMPLER2D(s_shadowMap1, 5);
SAMPLER2D(s_shadowMap2, 6);
SAMPLER2D(s_shadowMap3, 7);

///
uniform vec4 u_params[12];
#define u_mtx0          u_params[0]
#define u_mtx1          u_params[1]
#define u_mtx2          u_params[2]
#define u_mtx3          u_params[3]
#define u_glossiness    u_params[4].x
#define u_reflectivity  u_params[4].y
#define u_exposure      u_params[4].z
#define u_metalOrSpec   u_params[5].x
#define u_unused        u_params[5].yzw
#define u_doDiffuse     u_params[6].x
#define u_doSpecular    u_params[6].y
#define u_doDiffuseIbl  u_params[6].z
#define u_doSpecularIbl u_params[6].w
#define u_camPos        u_params[7].xyz
#define u_unused7       u_params[7].w
#define u_rgbDiff       u_params[8]
#define u_rgbSpec       u_params[9]
#define u_lightDir      u_params[10].xyz
#define u_unused10      u_params[10].w
#define u_lightCol      u_params[11].xyz
#define u_unused11      u_params[11].w

SAMPLERCUBE(s_texCube, 0);
SAMPLERCUBE(s_texCubeIrr, 1);


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

///

struct Shader
{
	vec3 ambi;
	vec3 diff;
	vec3 spec;
};

Shader evalShader(float _diff, float _spec)
{
	Shader shader;

	shader.ambi = u_lightAmbientPower.xyz  * u_lightAmbientPower.w  * u_materialKa.xyz;
	shader.diff = u_lightDiffusePower.xyz  * u_lightDiffusePower.w  * u_materialKd.xyz * _diff;
	shader.spec = u_lightSpecularPower.xyz * u_lightSpecularPower.w * u_materialKs.xyz * _spec;

	return shader;
}

float computeVisibility(sampler2D _sampler
					  , vec4 _shadowCoord
					  , float _bias
					  , vec4 _samplingParams
					  , vec2 _texelSize
					  , float _depthMultiplier
					  , float _minVariance
					  , float _hardness
					  )
{
	float visibility;

#if SM_LINEAR
	vec4 shadowcoord = vec4(_shadowCoord.xy / _shadowCoord.w, _shadowCoord.z, 1.0);
#else
	vec4 shadowcoord = _shadowCoord;
#endif

#if SM_HARD
	visibility = hardShadow(_sampler, shadowcoord, _bias);
#elif SM_PCF
	visibility = PCF(_sampler, shadowcoord, _bias, _samplingParams, _texelSize);
#elif SM_VSM
	visibility = VSM(_sampler, shadowcoord, _bias, _depthMultiplier, _minVariance);
#elif SM_ESM
	visibility = ESM(_sampler, shadowcoord, _bias, _depthMultiplier * _hardness);
#endif

	return visibility;
}
