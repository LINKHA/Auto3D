#pragma once

#include "../AutoConfig.h"

#ifdef AUTO_DIRECT3D_12
    #include "D3D11/D3D11Texture.h"
#endif
#ifdef AUTO_OPENGL
    #include "OGL/OGLTexture.h"
#endif