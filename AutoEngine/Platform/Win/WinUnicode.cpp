#include "WinUnicode.h"


// Old and dangerous interface.
void UTF8ToWide(const char* utf8, wchar_t* outBuffer, int outBufferSize)
{
	int res = ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, outBuffer, outBufferSize);
	if (res == 0)
		outBuffer[0] = 0;
}

