#pragma once

#include "../../AutoConfig.h"

namespace Auto3D
{
class Texture;

/// Change texture2d to texture cube
AUTO_API Texture* Texture2DtoTextureCube(Texture* texture2D);

}