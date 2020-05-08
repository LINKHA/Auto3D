$input a_position, a_normal
$output  v_normal, v_view, v_viewEnv, v_shadowcoord


#include "../common.sh"
#include "ibl_uniform.sh"

uniform mat4 u_lightMtx;
uniform vec4 u_params1;
#define u_shadowMapOffset u_params1.y

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
	v_shadowcoord.z += 0.5;
}
