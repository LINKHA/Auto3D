#pragma once

#include "Engine/Components/Image.h"

namespace Auto3D
{
#ifndef AUTO_OPENGL_ES
/// Decompress DXT1/3/5 image data.
AUTO_API void DecompressImageDXT(unsigned char* dest, const void* blocks, int width, int height, EImageFormat::Type _format);
/// Decompress ETC image data.
AUTO_API void DecompressImageETC(unsigned char* dest, const void* blocks, int width, int height);
/// Decompress PVRTC image data.
AUTO_API void DecompressImagePVRTC(unsigned char* dest, const void* blocks, int width, int height, EImageFormat::Type _format);
#endif
}
