#include "AutoString.h"

namespace Auto3D {

#ifdef _WIN32
WSTRING StringToWString(STRING str)
{
	WSTRING result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

STRING WStringToString(WSTRING wstr)
{
	STRING result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}
#else

STRING StringToWString(STRING str) {}

STRING WStringToString(STRING wstr) {}
#endif

void StringReplase(STRING& target, STRING& oldVal, STRING& newVal)
{
	STRING::size_type pos = 0;
	STRING::size_type oldSize = oldVal.size();
	STRING::size_type newSize = newVal.size();
	while ((pos = oldVal.find(newVal, pos)) != STRING::npos)
	{
		target.replace(pos, oldSize, newVal);
		pos += newSize;
	}
}


}

