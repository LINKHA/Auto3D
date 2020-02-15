$input a_position, a_normal, i_data0, i_data1, i_data2, i_data3
$output v_view, v_normal, v_shadowcoord

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common.sh"

uniform mat4 u_lightMtx;

void main()
{
	mat4 model;
	model[0] = i_data0;
	model[1] = i_data1;
	model[2] = i_data2;
	model[3] = i_data3;
	
	vec4 worldPos = instMul(model, vec4(a_position, 1.0) );
	
	gl_Position = mul(u_viewProj, worldPos );

	vec4 normal = a_normal * 2.0 - 1.0;
	v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);
	v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;

	const float shadowMapOffset = 0.001;
	vec3 posOffset = a_position + normal.xyz * shadowMapOffset;
	v_shadowcoord = mul(u_lightMtx, vec4(posOffset, 1.0) );
}
