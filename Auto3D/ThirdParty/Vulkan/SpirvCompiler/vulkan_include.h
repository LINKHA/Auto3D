#pragma once

#if _WIN32 || _WIN64
#include "vulkan/vulkan.h"
#elif __ANDROID__
#include "vulkan/vulkan_wrapper/vulkan_wrapper.h"
#elif IOS
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_ios.h"
#elif MAC
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_macos.h"
#endif
