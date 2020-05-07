$input a_position, a_normal
$output  v_normal, v_view, v_shadowcoord,v_viewEnv


#include "../common.sh"

uniform mat4 u_lightMtx;
uniform vec4 u_params1;


#define u_shadowMapOffset u_params1.y

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
	mat4 lightMtx = mul(u_lightMtx, u_model[0]);
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );

	vec4 normal = a_normal * 2.0 - 1.0;
	v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);

	v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
	v_viewEnv = u_camPos - mul(u_model[0], vec4(a_position, 1.0)).xyz;
	vec3 posOffset = a_position + normal.xyz * u_shadowMapOffset;
	v_shadowcoord = mul(lightMtx, vec4(posOffset, 1.0) );
}
