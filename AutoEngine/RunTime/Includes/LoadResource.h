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
Image * localImageLoad(PInt8 path);
ModelCommand localModelLoad(PInt8 path);

AUTO_END
#endif // !LOAD_ASSERT_H_
