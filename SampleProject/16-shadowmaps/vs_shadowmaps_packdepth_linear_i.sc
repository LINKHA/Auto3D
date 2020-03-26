$input a_position
$output v_depth


#include "../common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_depth = gl_Position.z * 0.5 + 0.5;
}
