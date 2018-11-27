#pragma once
#include <STB/stb_image.h>
#ifdef AUTO_IMAGE_WRITE_IMPL
#	define STB_IMAGE_WRITE_IMPLEMENTATION
#	include <STB/stb_image_write.h>
#endif

/**
* If you use write please define AUTO_IMAGE_WRITE_IMPL befor this header
* example : #include ...
			#include ...
			#define AUTO_IMAGE_WRITE
			#include "AutoImage.h"
*/