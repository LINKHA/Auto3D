$input a_position, a_texcoord0
$output v_texcoord0

#include "../common.sh"

void main()
{
    v_texcoord0 = a_texcoord0;
	gl_Position = vec4(a_position, 1.0);
}