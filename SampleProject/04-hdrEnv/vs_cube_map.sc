$input a_position, a_texcoord0
$output v_position

#include "../common.sh"

uniform mat4 u_environmentViewMatrix;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_position = a_position;
}
