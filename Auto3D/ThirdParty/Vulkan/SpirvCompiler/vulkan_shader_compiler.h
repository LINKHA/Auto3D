#pragma once

#include "vulkan_include.h"
#include <vector>
#include <string.h>

void InitShaderCompiler();
bool GlslToSpv(const VkShaderStageFlagBits shader_type, const char* src, std::vector<unsigned int>& spirv, std::string& error);
void DeinitShaderCompiler();
