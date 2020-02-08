$input a_position, a_normal, i_data0, i_data1, i_data2, i_data3
$output v_pos, v_view, v_normal, v_color0


#include "../common.sh"

void main()
{
	mat4 model;
	model[0] = i_data0;
	model[1] = i_data1;
	model[2] = i_data2;
	model[3] = i_data3;

	vec4 worldPos = instMul(model, vec4(a_position, 1.0) );

	vec3 normal = a_normal.xyz*2.0 - 1.0;

	gl_Position = mul(u_viewProj, worldPos );
	v_pos = gl_Position.xyz;

	v_view = mul(mul(model,u_view), vec4(a_position, 1.0) ).xyz;

	v_normal = mul(mul(model,u_view), vec4(normal, 0.0) ).xyz;

	v_color0 = vec4(1.0, 1.0, 1.0, 1.0);
}
