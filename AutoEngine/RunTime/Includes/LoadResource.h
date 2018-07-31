#pragma once
#include <string>
#include "ModelCommand.h"
#include "stb_image.h"
#include "stl_use.h"
#include "Image.h"

namespace Auto3D {

//Load Shader Address

//Load Resource Address 
unsigned int LocalTextureLoad(PInt8 path);
Image* LocalImageLoad(PInt8 path);
ModelCommand* LocalModelLoad(PInt8 path);
void FreeImage(Image * image);
unsigned int LoadCubemap(_VECTOR(_String) faces);
unsigned int LocalHdrLoad(PInt8 path);

}

