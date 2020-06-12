$input v_dir

#include "../common.sh"

SAMPLERCUBE(s_texCube, 0);
SAMPLERCUBE(s_texCubeIrr, 1);

void main()
{
	vec3 dir = normalize(v_dir);

	vec4 color;

	float lod  = 0.0;
	dir = fixCubeLookup(dir, lod, 256.0);
	color = toLinear(textureCubeLod(s_texCube, dir, lod));

	gl_FragColor = toFilmic(color);
}
