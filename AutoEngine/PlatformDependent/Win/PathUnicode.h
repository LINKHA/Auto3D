#pragma once
#include "WinUnicode.h"
#include <algorithm>

void ConvertWindowsPathName(const wchar_t* widePath, char* outBuffer, int outBufferSize);

void ConvertAutoPathName(const char* utf8, wchar_t* outBuffer, int outBufferSize);
