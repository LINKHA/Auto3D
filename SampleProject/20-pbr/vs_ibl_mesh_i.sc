$input a_position, a_normal, i_data0, i_data1, i_data2, i_data3
$output v_view, v_normal

#include "../common.sh"
#include "uniforms.sh"

void main()
{
	mat4 model;
	model = mtxFromCols(i_data0 , i_data1 , i_data2 , i_data3);
	mat4 modelView = mul(u_view,model);
	mat4 modelViewProj = mul(u_viewProj,model);
	
	gl_Position = mul(modelViewProj, vec4(a_position, 1.0) );

	v_view = u_camPos - mul(model, vec4(a_position, 1.0)).xyz;

	vec3 normal = a_normal * 2.0 - 1.0;
	v_normal = mul(u_model[0], vec4(normal, 0.0) ).xyz;
}
