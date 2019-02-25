#pragma once

#include "Image.h"

namespace Auto3D
{

/// Decompress DXT1/3/5 image data.
AUTO_API void DecompressImageDXT(unsigned char* dest, const void* blocks, int width, int height, ImageFormat format);
/// Decompress ETC image data.
AUTO_API void DecompressImageETC(unsigned char* dest, const void* blocks, int width, int height);
/// Decompress PVRTC image data.
AUTO_API void DecompressImagePVRTC(unsigned char* dest, const void* blocks, int width, int height, ImageFormat format);

}
