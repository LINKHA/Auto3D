#include "PathUnicode.h"

namespace detail {

	static void ConvertSeparatorsToUnity(char* pathName)
	{
		while (*pathName != '\0') {
			if (*pathName == '\\')
				*pathName = '/';
			++pathName;
		}
	}

	static void ConvertSeparatorsToWindows(wchar_t* pathName)
	{
		while (*pathName != L'\0') {
			if (*pathName == L'/')
				*pathName = L'\\';
			++pathName;
		}
	}

} // namespace detail

void ConvertWindowsPathName(const wchar_t* widePath, char* outBuffer, int outBufferSize)
{
	::WideCharToMultiByte(CP_UTF8, 0, widePath, -1, outBuffer, outBufferSize, NULL, NULL);
	detail::ConvertSeparatorsToUnity(outBuffer);
}

void ConvertAutoPathName(const char* utf8, wchar_t* outBuffer, int outBufferSize)
{
	UTF8ToWide(utf8, outBuffer, outBufferSize);
	detail::ConvertSeparatorsToWindows(outBuffer);
}


