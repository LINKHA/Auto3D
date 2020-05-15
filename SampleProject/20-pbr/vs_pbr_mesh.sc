$input a_position, a_normal, a_texcoord0
$output v_view, v_normal, v_worldPos, v_texcoord0

#include "../common.sh"
#include "uniforms.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );

	v_view = u_camPos - mul(u_model[0], vec4(a_position, 1.0)).xyz;

	vec3 normal = a_normal * 2.0 - 1.0;
	v_normal = mul(u_model[0], vec4(normal, 0.0) ).xyz;
	v_worldPos = mul(u_model[0] , vec4(a_position, 1.0)).xyz;

	v_texcoord0 = a_texcoord0;
}
