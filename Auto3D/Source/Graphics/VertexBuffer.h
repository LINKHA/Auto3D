#pragma once

#include "../AutoConfig.h"

#ifdef AUTO_DIRECT3D_12
    #include "D3D12/D3D12VertexBuffer.h"
#endif
#ifdef AUTO_OPENGL
    #include "OGL/OGLVertexBuffer.h"
#endif