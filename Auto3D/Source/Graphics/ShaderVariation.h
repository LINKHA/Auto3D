#pragma once

#include "../AutoConfig.h"

#ifdef AUTO_DIRECT3D_12
    #include "D3D12/D3D12ShaderVariation.h"
#endif
#ifdef AUTO_OPENGL
    #include "OGL/OGLShaderVariation.h"
#endif
