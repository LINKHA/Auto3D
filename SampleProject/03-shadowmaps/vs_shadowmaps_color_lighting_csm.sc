$input a_position, a_normal
$output v_position, v_normal, v_view,v_viewEnv, v_texcoord1, v_texcoord2, v_texcoord3, v_texcoord4

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common.sh"

uniform vec4 u_params1;
#define u_shadowMapOffset u_params1.y

uniform mat4 u_lightMtx;
uniform mat4 u_shadowMapMtx0;
uniform mat4 u_shadowMapMtx1;
uniform mat4 u_shadowMapMtx2;
uniform mat4 u_shadowMapMtx3;

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

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );

	vec4 normal = a_normal * 2.0 - 1.0;
	v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);
	v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
	v_viewEnv = u_camPos - mul(u_model[0], vec4(a_position, 1.0)).xyz;

	vec4 posOffset = vec4(a_position + normal.xyz * u_shadowMapOffset, 1.0);
	v_position = mul(u_modelView, posOffset);

	vec4 wpos = vec4(mul(u_model[0], posOffset).xyz, 1.0);
	v_texcoord1 = mul(u_shadowMapMtx0, wpos);
	v_texcoord2 = mul(u_shadowMapMtx1, wpos);
	v_texcoord3 = mul(u_shadowMapMtx2, wpos);
	v_texcoord4 = mul(u_shadowMapMtx3, wpos);
}
