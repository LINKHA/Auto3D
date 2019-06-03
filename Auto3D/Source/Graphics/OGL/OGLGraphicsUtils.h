#pragma once

#include "../../AutoConfig.h"
#include "../Texture.h"

namespace Auto3D
{

/// Change texture2d to texture cube
inline AUTO_API Texture* Texture2DtoTextureCube(Texture* texture2D);

}