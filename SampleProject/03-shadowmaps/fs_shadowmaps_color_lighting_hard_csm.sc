$input v_position, v_normal, v_view, v_viewEnv, v_texcoord1, v_texcoord2, v_texcoord3, v_texcoord4

#define SM_HARD 1
#define SM_CSM 1

#include "fs_shadowmaps_color_lighting.sh"

void main()
{
#include "fs_shadowmaps_color_lighting_main.sh"
}
