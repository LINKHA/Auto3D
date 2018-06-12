#ifndef LOAD_RESOURCE_H_
#define LOAD_RESOURCE_H_
#include <string>
#include "ModelCommand.h"
#include "stb_image.h"
#include "stl_use.h"
#include "Image.h"

AUTO_BEGIN




//Load Shader Address

//Load Resource Address 
unsigned int LocalTextureLoad(PInt8 path);
Image * LocalImageLoad(PInt8 path);
ModelCommand LocalModelLoad(PInt8 path);
void FreeImage(Image * image);

AUTO_END
#endif // !LOAD_ASSERT_H_
