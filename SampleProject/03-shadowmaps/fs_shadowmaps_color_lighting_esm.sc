$input v_normal, v_view, v_shadowcoord, v_viewEnv


#define SM_ESM 1

#include "fs_shadowmaps_color_lighting.sh"

void main()
{
#include "fs_shadowmaps_color_lighting_main.sh"
}
