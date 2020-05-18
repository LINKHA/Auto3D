$input a_position, a_texcoord0
$output v_dir

#include "../common.sh"

uniform mat4 u_environmentViewMatrix;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );

	float fov = radians(45.0);
	float height = tan(fov*0.5);
	float aspect = height*(u_viewRect.z / u_viewRect.w);
	vec2 tex = (2.0*a_texcoord0-1.0) * vec2(aspect, height);

	v_dir = instMul(u_environmentViewMatrix, vec4(tex, 1.0, 0.0) ).xyz;
}
