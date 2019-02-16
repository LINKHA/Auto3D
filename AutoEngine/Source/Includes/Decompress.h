#pragma once
#include "Image.h"

namespace Auto3D {

/// Decompress a DXT compressed image to RGBA.
void DecompressImageDXT(unsigned char* rgba, const void* blocks, int width, int height, int depth, CompressedFormat format);
/// Decompress an ETC1 compressed image to RGBA.
void DecompressImageETC(unsigned char* rgba, const void* blocks, int width, int height);
/// Decompress a PVRTC compressed image to RGBA.
void DecompressImagePVRTC(unsigned char* rgba, const void* blocks, int width, int height, CompressedFormat format);
/// Flip a compressed block vertically.
void FlipBlockVertical(unsigned char* dest, const unsigned char* src, CompressedFormat format);
/// Flip a compressed block horizontally.
void FlipBlockHorizontal(unsigned char* dest, const unsigned char* src, CompressedFormat format);


}