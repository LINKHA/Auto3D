$input a_position, a_normal, i_data0, i_data1, i_data2, i_data3
$output v_position, v_normal, v_view, v_texcoord1, v_texcoord2, v_texcoord3, v_texcoord4

#include "../common.sh"

uniform vec4 u_params1;
#define u_shadowMapOffset u_params1.y

uniform mat4 u_lightMtx;
uniform mat4 u_shadowMapMtx0;
uniform mat4 u_shadowMapMtx1;
uniform mat4 u_shadowMapMtx2;
uniform mat4 u_shadowMapMtx3;

void main()
{
	mat4 model;
	model[0] = i_data0;
	model[1] = i_data1;
	model[2] = i_data2;
	model[3] = i_data3;

	vec4 worldPos = instMul(model, vec4(a_position, 1.0) );

	mat4 modelView = mul(u_view , model);

	gl_Position = mul(u_viewProj, worldPos);

	vec4 normal = a_normal * 2.0 - 1.0;
	v_normal = normalize(mul(modelView, vec4(normal.xyz, 0.0) ).xyz);
	v_view = mul(modelView, vec4(a_position, 1.0)).xyz;

	vec4 posOffset = vec4(a_position + normal.xyz * u_shadowMapOffset, 1.0);
	v_position = mul(modelView, posOffset);

	vec4 wpos = vec4(mul(model, posOffset).xyz, 1.0);
	v_texcoord1 = mul(u_shadowMapMtx0, wpos);
	v_texcoord2 = mul(u_shadowMapMtx1, wpos);
	v_texcoord3 = mul(u_shadowMapMtx2, wpos);
	v_texcoord4 = mul(u_shadowMapMtx3, wpos);
}
