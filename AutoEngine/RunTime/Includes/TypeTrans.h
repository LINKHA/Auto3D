#pragma once
#include "string.h"
#include "malloc.h"
#include <stdlib.h>

static wchar_t* CStrToWStr(char* cstr)
{
	size_t len = strlen(cstr) + 1;
	size_t converted = 0;
	wchar_t *wstr;
	wstr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, wstr, len, cstr, _TRUNCATE);
	return wstr;
}
