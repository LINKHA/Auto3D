$input v_depth

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common.sh"

void main()
{
	float depth = v_depth;
	float depthSq = depth*depth;

	gl_FragColor = vec4(packHalfFloat(depth), packHalfFloat(depthSq));
}
