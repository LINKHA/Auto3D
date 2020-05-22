$input v_dir

#include "../common.sh"

SAMPLERCUBE(s_equirectangulaCubeMap, 0);

void main()
{
	vec3 dir = normalize(v_dir);

	vec4 color;

	//color = toLinear(textureCube(s_equirectangulaCubeMap, dir));

	color = textureCube(s_equirectangulaCubeMap, dir);

	gl_FragColor = toFilmic(color);
}
