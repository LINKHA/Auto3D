#include "TypeString.h"

namespace KhSTL {

const tString tString::EMPTY;

tWString::tWString(const tString& str)
	: _length(0)
	, _buffer(nullptr)
{
#ifdef _WIN32
	unsigned neededSize = 0;
	wchar_t temp[3];

	unsigned byteOffset = 0;
	while (byteOffset < str.Length())
	{
		wchar_t* dest = temp;
		tString::EncodeUTF16(dest, str.NextUTF8Char(byteOffset));
		neededSize += dest - temp;
	}

	Resize(neededSize);

	byteOffset = 0;
	wchar_t* dest = _buffer;
	while (byteOffset < str.Length())
		tString::EncodeUTF16(dest, str.NextUTF8Char(byteOffset));
#else
	Resize(str.LengthUTF8());

	unsigned byteOffset = 0;
	wchar_t* dest = buffer_;
	while (byteOffset < str.Length())
		*dest++ = (wchar_t)str.NextUTF8Char(byteOffset);
#endif
}

std::istream& operator >>(std::istream &in, tString& rhs)
{
	in >> rhs._buffer;
	return in;
}

std::ostream& operator <<(std::ostream& out, tString& rhs)
{
	out << rhs._buffer;
	return out;
}

std::wistream& operator >>(std::wistream &in, tWString& rhs)
{
	in >> rhs._buffer;
	return in;
}

std::wostream& operator <<(std::wostream& out, tWString& rhs)
{
	out << rhs._buffer;
	return out;
}

size_t tString::CountElements(const char* buffer, char separator)
{
	if (!buffer)
		return 0;

	const char* endPos = buffer + tString::CStrLength(buffer);
	const char* pos = buffer;
	size_t ret = 0;

	while (pos < endPos)
	{
		if (*pos != separator)
			break;
		++pos;
	}

	while (pos < endPos)
	{
		const char* start = pos;

		while (start < endPos)
		{
			if (*start == separator)
				break;

			++start;
		}

		if (start == endPos)
		{
			++ret;
			break;
		}

		const char* end = start;

		while (end < endPos)
		{
			if (*end != separator)
				break;

			++end;
		}

		++ret;
		pos = end;
	}

	return ret;
}



}