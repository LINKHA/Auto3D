#ifndef KH_STL_TYPE_STRING_H_
#define KH_STL_TYPE_STRING_H_


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <cassert>
#include <cstdarg>
#include <iostream>
#include <vector>

#include "../../Utility/TypeIterator.h"
#include "../../Algorithm/TypeAlgorithm.h"

namespace KhSTL {

class tString;

#define GET_NEXT_CONTINUATION_BYTE(ptr) *(ptr); if ((unsigned char)*(ptr) < 0x80 || (unsigned char)*(ptr) >= 0xc0) return '?'; else ++(ptr);
#define Khs(value)	KhSTL::tString(value)
#define Khts(value)	KhSTL::tWString(value)
static const int BUFFER_LENGTH = 128;

static const int CONVERSION_BUFFER_LENGTH = 128;
static const int MATRIX_CONVERSION_BUFFER_LENGTH = 256;


class tWString
{
public:
	/**
	* @brief : Construct Empty
	*/
	tWString()
		: _length(0)
		, _buffer(nullptr)
	{}
	/**
	* @brief : Construct from a string
	*/
	explicit tWString(const tString& str);
	/**
	* @brief : Destruct
	*/
	~tWString()
	{
		delete[] _buffer;
	}
	/**
	* @brief : Right shift import istream
	*/
	friend std::wistream& operator >>(std::wistream &in, tWString& rhs);
	/**
	* @brief : Left shift import ostream
	*/
	friend std::wostream& operator <<(std::wostream &out, tWString& rhs);

	/**
	* @brief : Return char at index
	*/
	wchar_t& operator [](unsigned index)
	{
		assert(index < _length);
		return _buffer[index];
	}

	/**
	* @brief : Return const char at index
	*/
	const wchar_t& operator [](unsigned index) const
	{
		assert(index < _length);
		return _buffer[index];
	}

	/**
	* @brief : Return char at index
	*/
	wchar_t& At(unsigned index)
	{
		assert(index < _length);
		return _buffer[index];
	}
	/**
	* @brief : Return const char at index
	*/
	const wchar_t& At(unsigned index) const
	{
		assert(index < _length);
		return _buffer[index];
	}
	/**
	* @brief : Resize the string
	*/
	void Resize(unsigned newLength)
	{
		if (!newLength)
		{
			delete[] _buffer;
			_buffer = nullptr;
			_length = 0;
		}
		else
		{
			auto* newBuffer = new wchar_t[newLength + 1];
			if (_buffer)
			{
				unsigned copyLength = _length < newLength ? _length : newLength;
				memcpy(newBuffer, _buffer, copyLength * sizeof(wchar_t));
				delete[] _buffer;
			}
			newBuffer[newLength] = 0;
			_buffer = newBuffer;
			_length = newLength;
		}
	}
	/**
	* @brief : Return whether the string is Empty
	*/
	bool Empty() const { return _length == 0; }
	/**
	* @brief : Return length
	*/
	unsigned Length() const { return _length; }
	/**
	* @brief : Return character data
	*/
	const wchar_t* CStr() const { return _buffer; }

	/// String length
	unsigned _length;
	/// String buffer, null if not allocated
	wchar_t* _buffer;
};

/**
* String class for LINKH STL
*/
class tString
{
	using Iterator = tIterator<char>;
	using ConstIterator = tConstIterator<char>;
public:
	/**
	* @brief : Construct
	*/
	tString() noexcept
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{}

	/**
	* @brief : Construct from another string
	*/
	tString(const tString& str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		*this = str;
	}
	/**
	* @brief : Move-construct from another string
	*/
	tString(tString && str) noexcept
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		Swap(str);
	}
	/**
	* @brief : Construct from the C string
	*/
	tString(const char* str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		*this = str;
	}
	/**
	* @brief : Construct from the C string
	*/
	tString(char* str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		*this = (const char*)str;
	}
	/**
	* @brief : Construct from the char array and length
	*/
	tString(const char* str, unsigned length)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		Resize(length);
		CopyChars(_buffer, str, length);
	}
	/**
	* @brief : Construction of the wide character array from the null end
	*/
	explicit tString(const wchar_t* str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		SetUTF8FromWChar(str);
	}
	/**
	* @brief : Constructed from the wide array of characters ending in null
	*/
	explicit tString(wchar_t* str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		SetUTF8FromWChar(str);
	}
	/**
	* @brief : Construct from the WString
	*/
	explicit tString(const tWString& str)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		SetUTF8FromWChar(str.CStr());
	}
	/**
	* @brief : Construct from the int
	*/
	explicit tString(int value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%d", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the unsigned short
	*/
	explicit tString(unsigned short value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%u", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the short
	*/
	explicit tString(short value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%d", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the long
	*/
	explicit tString(long value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%ld", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the long long
	*/
	explicit tString(long long value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%lld", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the unsigned
	*/
	explicit tString(unsigned value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%u", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the unsigned long
	*/
	explicit tString(unsigned long value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%lu", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the unsigned long long
	*/
	explicit tString(unsigned long long value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%llu", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the float
	*/
	explicit tString(float value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%g", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the double
	*/
	explicit tString(double value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%.15g", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the bool
	*/
	explicit tString(long double value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		char cBuffer[BUFFER_LENGTH];
		sprintf(cBuffer, "%.15lg", value);
		*this = cBuffer;
	}
	/**
	* @brief : Construct from the bool
	*/
	explicit tString(bool value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		if (value)
			*this = "true";
		else
			*this = "false";
	}
	/**
	* @brief : Construct from the char
	*/
	explicit tString(char value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		Resize(1);
		_buffer[0] = value;
	}
	/**
	* @brief : Construct from the char and unsigned length.
	*/
	tString(char value, unsigned length)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		Resize(length);
		for (unsigned i = 0; i < length; ++i)
			_buffer[i] = value;
	}
	/**
	* @brief : Construct from the convertible value
	*/
	template <typename T> explicit tString(const T& value)
		: _length(0)
		, _capacity(0)
		, _buffer(&endZero)
	{
		*this = value.ToString();
	}
	/**
	* @brief : Destruct
	*/
	~tString()
	{
		if (_capacity)
			delete[] _buffer;
	}

	/**
	* @brief : Assign the string.
	*/
	tString& operator =(const tString& rhs)
	{
		if (&rhs != this)
		{
			Resize(rhs._length);
			CopyChars(_buffer, rhs._buffer, rhs._length);
		}
		return *this;
	}
	/**
	* @brief : Move assign the string.
	*/
	tString& operator =(tString && rhs) noexcept
	{
		assert(&rhs != this);
		Swap(rhs);
		return *this;
	}
	/**
	* @brief : Assign the C str
	*/
	tString& operator =(const char* rhs)
	{
		unsigned rhsLength = CStrLength(rhs);
		Resize(rhsLength);
		CopyChars(_buffer, rhs, rhsLength);
		return *this;
	}
	/**
	* @brief : Add assign the string
	*/
	tString& operator +=(const tString& rhs)
	{
		unsigned oldLength = _length;
		Resize(_length + rhs._length);
		CopyChars(_buffer + oldLength, rhs._buffer, rhs._length);
		return *this;
	}

	/**
	* @brief : Add assign the C str
	*/
	tString& operator +=(const char* rhs)
	{
		unsigned rhsLength = CStrLength(rhs);
		unsigned oldLength = _length;
		Resize(_length + rhsLength);
		CopyChars(_buffer + oldLength, rhs, rhsLength);
		return *this;
	}
	/**
	* @brief : Add assign the char
	*/
	tString& operator +=(char rhs)
	{
		unsigned oldLength = _length;
		Resize(_length + 1);
		_buffer[oldLength] = rhs;
		return *this;
	}
	/**
	* @brief : Add assign the int(concatenate as string)
	*/
	tString& operator +=(int rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the short(concatenate as string)
	*/
	tString& operator +=(short rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the long(concatenate as string)
	*/
	tString& operator +=(long rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the long long(concatenate as string)
	*/
	tString& operator +=(long long rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the unsigned(concatenate as string)
	*/
	tString& operator +=(unsigned rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the short unsigned(concatenate as string)
	*/
	tString& operator +=(unsigned short rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the long unsigned(concatenate as string)
	*/
	tString& operator +=(unsigned long rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the long long unsigned(concatenate as string)
	*/
	tString& operator +=(unsigned long long rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the float(concatenate as string)
	*/
	tString& operator +=(float rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the bool(concatenate as string)
	*/
	tString& operator +=(bool rhs)
	{
		return *this += tString(rhs);
	}
	/**
	* @brief : Add assign the arbitrary type(concatenate as string)
	*/
	template <class T> tString& operator +=(const T& rhs)
	{
		return *this += rhs.ToString();
	}
	/**
	* @brief : Add the string
	*/
	tString operator +(const tString& rhs) const
	{
		tString str;
		str.Resize(_length + rhs._length);
		CopyChars(str._buffer, _buffer, _length);
		CopyChars(str._buffer + _length, rhs._buffer, rhs._length);

		return str;
	}
	/**
	* @brief : Add the C str
	*/
	tString operator +(const char* rhs) const
	{
		unsigned rhsLength = CStrLength(rhs);
		tString str;
		str.Resize(_length + rhsLength);
		CopyChars(str._buffer, _buffer, _length);
		CopyChars(str._buffer + _length, rhs, rhsLength);

		return str;
	}
	/**
	* @brief : Test for equality with another string
	*/
	bool operator ==(const tString& rhs) const
	{
		return strcmp(CStr(), rhs.CStr()) == 0;
	}
	/**
	* @brief : Test for inequality with another string.
	*/
	bool operator !=(const tString& rhs) const
	{
		return strcmp(CStr(), rhs.CStr()) != 0;
	}
	/**
	* @brief : Test if string is less than another string
	*/
	bool operator <(const tString& rhs) const
	{
		return strcmp(CStr(), rhs.CStr()) < 0;
	}
	/**
	* @brief : Test if string is greater than another string
	*/
	bool operator >(const tString& rhs) const
	{
		return strcmp(CStr(), rhs.CStr()) > 0;
	}
	/**
	* @brief : Test for equality with the C string
	*/
	bool operator ==(const char* rhs) const
	{
		return strcmp(CStr(), rhs) == 0;
	}
	/**
	* @brief : Test for inequality with the C string
	*/
	bool operator !=(const char* rhs) const
	{
		return strcmp(CStr(), rhs) != 0;
	}
	/**
	* @brief : Test if string is less than the C string
	*/
	bool operator <(const char* rhs) const
	{
		return strcmp(CStr(), rhs) < 0;
	}
	/**
	* @brief : Test if string is greater than the C string
	*/
	bool operator >(const char* rhs) const
	{
		return strcmp(CStr(), rhs) > 0;
	}
	/**
	* @brief : Right shift import istream
	*/
	friend std::istream& operator >>(std::istream &in, tString& rhs);
	/**
	* @brief : Left shift import ostream
	*/
	friend std::ostream& operator <<(std::ostream &out, tString& rhs);

	/**
	* @brief :  Return char at index
	*/
	char& operator [](unsigned index)
	{
		assert(index < _length);
		return _buffer[index];
	}
	/**
	* @brief : Return const char at index
	*/
	const char& operator [](unsigned index) const
	{
		assert(index < _length);
		return _buffer[index];
	}

	/**
	* @brief : Return char at index
	*/
	char& At(unsigned index)
	{
		assert(index < _length);
		return _buffer[index];
	}
	/**
	* @brief : Return char at index
	*/
	const char& At(unsigned index) const
	{
		assert(index < _length);
		return _buffer[index];
	}
	/**
	* @brief : Replace substring with another substring
	*/
	void Replace(unsigned pos, unsigned length, const char* srcStart, unsigned srcLength)
	{
		int delta = (int)srcLength - (int)length;

		if (pos + length < _length)
		{
			if (delta < 0)
			{
				MoveRange(pos + srcLength, pos + length, _length - pos - length);
				Resize(_length + delta);
			}
			if (delta > 0)
			{
				Resize(_length + delta);
				MoveRange(pos + srcLength, pos + length, _length - pos - length - delta);
			}
		}
		else
			Resize(_length + delta);

		CopyChars(_buffer + pos, srcStart, srcLength);
	}
	/**
	* @brief : Replace all occurrences of the char
	*/
	void Replace(char replaceTarget, char replaceParam, bool caseSensitive = true)
	{
		if (caseSensitive)
		{
			for (unsigned i = 0; i < _length; ++i)
			{
				if (_buffer[i] == replaceTarget)
					_buffer[i] = replaceParam;
			}
		}
		else
		{
			replaceTarget = (char)tolower(replaceTarget);
			for (unsigned i = 0; i < _length; ++i)
			{
				if (tolower(_buffer[i]) == replaceTarget)
					_buffer[i] = replaceParam;
			}
		}
	}
	/**
	* @brief : Replace all occurrences of the string
	*/
	void Replace(const tString& replaceTarget, const tString& replaceParam, bool caseSensitive = true)
	{
		unsigned nextPos = 0;

		while (nextPos < _length)
		{
			unsigned pos = Find(replaceTarget, nextPos, caseSensitive);
			if (pos == NO_POS)
				break;
			Replace(pos, replaceTarget._length, replaceParam);
			nextPos = pos + replaceParam._length;
		}
	}
	/**
	* @brief : Replace substring
	*/
	void Replace(unsigned pos, unsigned length, const tString& replaceParam)
	{
		// If substring is illegal, do nothing
		if (pos + length > _length)
			return;

		Replace(pos, length, replaceParam._buffer, replaceParam._length);
	}
	/**
	* @brief : Replace substring with a C string
	*/
	void Replace(unsigned pos, unsigned length, const char* replaceParam)
	{
		// If substring is illegal, do nothing
		if (pos + length > _length)
			return;

		Replace(pos, length, replaceParam, CStrLength(replaceParam));
	}
	/**
	* @brief : Replace substring by iterators
	*/
	Iterator Replace(const Iterator& start, const Iterator& end, const tString& replaceParam)
	{
		unsigned pos = (unsigned)(start - Begin());
		if (pos >= _length)
			return End();
		auto length = (unsigned)(end - start);
		Replace(pos, length, replaceParam);

		return Begin() + pos;
	}
	/**
	* @brief : Return string with all occurrences of the character replaced
	*/
	tString Replaced(char replaceTarget, char replaceParam, bool caseSensitive = true) const
	{
		tString ret(*this);
		ret.Replace(replaceTarget, replaceParam, caseSensitive);
		return ret;
	}
	/**
	* @brief : Return string with all occurrences of the string replaced
	*/
	tString Replaced(const tString& replaceTarget, const tString& replaceParam, bool caseSensitive = true) const
	{
		tString ret(*this);
		ret.Replace(replaceTarget, replaceParam, caseSensitive);
		return ret;
	}
	/**
	* @brief : Append the string
	*/
	tString& Append(const tString& str)
	{
		return *this += str;
	}
	/**
	* @brief : Append the C string
	*/
	tString& Append(const char* str)
	{
		return *this += str;
	}
	/**
	* @brief : Append the char
	*/
	tString& Append(char c)
	{
		return *this += c;
	}

	/**
	* @brief : Append char
	*/
	tString& Append(const char* str, unsigned length)
	{
		if (str)
		{
			unsigned oldLength = _length;
			Resize(oldLength + length);
			CopyChars(&_buffer[oldLength], str, length);
		}
		return *this;
	}

	/**
	* @brief : Insert string
	*/
	void Insert(unsigned pos, const tString& str)
	{
		if (pos > _length)
			pos = _length;

		if (pos == _length)
			(*this) += str;
		else
			Replace(pos, 0, str);
	}
	/**
	* @brief : Insert character
	*/
	void Insert(unsigned pos, char c)
	{
		if (pos > _length)
			pos = _length;

		if (pos == _length)
			(*this) += c;
		else
		{
			unsigned oldLength = _length;
			Resize(_length + 1);
			MoveRange(pos + 1, pos, oldLength - pos);
			_buffer[pos] = c;
		}
	}

	/**
	* @brief : Insert string by iterator
	*/
	Iterator Insert(const Iterator& dest, const tString& str)
	{
		unsigned pos = (unsigned)(dest - Begin());
		if (pos > _length)
			pos = _length;
		Insert(pos, str);

		return Begin() + pos;
	}
	/**
	* @brief : Insert the string partially by iterators
	*/
	Iterator Insert(const Iterator& dest, const Iterator& start, const Iterator& end)
	{
		unsigned pos = (unsigned)(dest - Begin());
		if (pos > _length)
			pos = _length;
		auto length = (unsigned)(end - start);
		Replace(pos, 0, &(*start), length);

		return Begin() + pos;
	}

	/**
	* @brief : Insert the character by iterator
	*/
	Iterator Insert(const Iterator& dest, char c)
	{
		unsigned pos = (unsigned)(dest - Begin());
		if (pos > _length)
			pos = _length;
		Insert(pos, c);

		return Begin() + pos;
	}
	/**
	* @brief : Erase substring
	*/
	void Erase(unsigned pos, unsigned length = 1)
	{
		Replace(pos, length, tString::EMPTY);
	}

	/**
	* @brief : Erase char by iterator
	*/
	Iterator Erase(const Iterator& it)
	{
		unsigned pos = (unsigned)(it - Begin());
		if (pos >= _length)
			return End();
		Erase(pos);

		return Begin() + pos;
	}
	/**
	* @brief : Erase substring by iterators
	*/
	Iterator Erase(const Iterator& start, const Iterator& end)
	{
		unsigned pos = (unsigned)(start - Begin());
		if (pos >= _length)
			return End();
		auto length = (unsigned)(end - start);
		Erase(pos, length);

		return Begin() + pos;
	}
	/**
	* @brief : Resize the string
	*/
	void Resize(unsigned newLength)
	{
		if (!_capacity)
		{
			if (!newLength)
				return;

			// Calculate initial capacity
			_capacity = newLength + 1;
			if (_capacity < MIN_CAPACITY)
				_capacity = MIN_CAPACITY;

			_buffer = new char[_capacity];
		}
		else
		{
			if (newLength && _capacity < newLength + 1)
			{
				// Increase the capacity with half each time it is exceeded
				while (_capacity < newLength + 1)
					_capacity += (_capacity + 1) >> 1u;

				auto* newBuffer = new char[_capacity];
				// Move the existing data to the new buffer, then delete the old buffer
				if (_length)
					CopyChars(newBuffer, _buffer, _length);
				delete[] _buffer;

				_buffer = newBuffer;
			}
		}

		_buffer[newLength] = 0;
		_length = newLength;
	}
	/**
	* @brief : Set new capacity
	*/
	void Reserve(unsigned newCapacity)
	{
		if (newCapacity < _length + 1)
			newCapacity = _length + 1;
		if (newCapacity == _capacity)
			return;

		auto* newBuffer = new char[newCapacity];
		// Move the existing data to the new buffer, then delete the old buffer
		CopyChars(newBuffer, _buffer, _length + 1);
		if (_capacity)
			delete[] _buffer;

		_capacity = newCapacity;
		_buffer = newBuffer;
	}
	/**
	* @brief : Reallocate so that no extra memory is used
	*/
	void Compact()
	{
		if (_capacity)
			Reserve(_length + 1);
	}
	/**
	* @brief : Clear the string
	*/
	void Clear()
	{
		Resize(0);
	}
	/**
	* @brief : Swap with another string
	*/
	void Swap(tString& str)
	{
		KhSTL::Swap(_length, str._length);
		KhSTL::Swap(_capacity, str._capacity);
		KhSTL::Swap(_buffer, str._buffer);
	}
	/**
	* @brief : Return iterator to the beginning
	*/
	Iterator Begin()
	{
		return Iterator(_buffer);
	}
	/**
	* @brief : Return const iterator to the beginning
	*/
	ConstIterator Begin() const
	{
		return ConstIterator(_buffer);
	}
	/**
	* @brief : Return iterator to the end
	*/
	Iterator End()
	{
		return Iterator(_buffer + _length);
	}
	/**
	* @brief : Return const iterator to the end
	*/
	ConstIterator End() const
	{
		return ConstIterator(_buffer + _length);
	}
	/**
	* @brief : Return first char, or 0 if Empty
	*/
	char Front() const
	{
		return _buffer[0];
	}
	/**
	* @brief : Return last char, or 0 if Empty
	*/
	char Back() const
	{
		return _length ? _buffer[_length - 1] : _buffer[0];
	}
	/**
	* @brief : Return a substring from position to end
	*/
	tString SubString(unsigned pos) const
	{
		if (pos < _length)
		{
			tString ret;
			ret.Resize(_length - pos);
			CopyChars(ret._buffer, _buffer + pos, ret._length);

			return ret;
		}
		else
			return tString();
	}
	/**
	* @brief : Return a substring with length from position
	*/
	tString SubString(unsigned pos, unsigned length) const
	{
		if (pos < _length)
		{
			tString ret;
			if (pos + length > _length)
				length = _length - pos;
			ret.Resize(length);
			CopyChars(ret._buffer, _buffer + pos, ret._length);

			return ret;
		}
		else
			return tString();
	}
	/**
	* @brief : Return string with whitespace trimmed from the beginning and the end
	*/
	tString Trimmed() const
	{
		unsigned trimStart = 0;
		unsigned trimEnd = _length;

		while (trimStart < trimEnd)
		{
			char c = _buffer[trimStart];
			if (c != ' ' && c != 9)
				break;
			++trimStart;
		}
		while (trimEnd > trimStart)
		{
			char c = _buffer[trimEnd - 1];
			if (c != ' ' && c != 9)
				break;
			--trimEnd;
		}

		return SubString(trimStart, trimEnd - trimStart);
	}
	/**
	* @brief : Return string in uppercase
	*/
	tString ToUpper() const
	{
		tString ret(*this);
		for (unsigned i = 0; i < ret._length; ++i)
			ret[i] = (char)toupper(_buffer[i]);

		return ret;
	}
	/**
	* @brief : Return string in lowercase
	*/
	tString ToLower() const
	{
		tString ret(*this);
		for (unsigned i = 0; i < ret._length; ++i)
			ret[i] = (char)tolower(_buffer[i]);

		return ret;
	}
	/**
	* @brief : Return substrings split by a separator char. By default don't return Empty strings
	*/
	std::vector<tString> Split(char separator, bool keepEmptyStrings = false) const
	{
		return Split(CStr(), separator, keepEmptyStrings);
	}
	/**
	* @brief : Return index to the first occurrence of a character, or NO_POS if not found
	*/
	unsigned Find(char c, unsigned startPos = 0, bool caseSensitive = true) const
	{
		if (caseSensitive)
		{
			for (unsigned i = startPos; i < _length; ++i)
			{
				if (_buffer[i] == c)
					return i;
			}
		}
		else
		{
			c = (char)tolower(c);
			for (unsigned i = startPos; i < _length; ++i)
			{
				if (tolower(_buffer[i]) == c)
					return i;
			}
		}

		return NO_POS;
	}
	/**
	* @brief : Return index to the first occurrence of a string, or NO_POS if not found
	*/
	unsigned Find(const tString& str, unsigned startPos = 0, bool caseSensitive = true) const
	{
		if (!str._length || str._length > _length)
			return NO_POS;

		char first = str._buffer[0];
		if (!caseSensitive)
			first = (char)tolower(first);

		for (unsigned i = startPos; i <= _length - str._length; ++i)
		{
			char c = _buffer[i];
			if (!caseSensitive)
				c = (char)tolower(c);

			if (c == first)
			{
				unsigned skip = NO_POS;
				bool found = true;
				for (unsigned j = 1; j < str._length; ++j)
				{
					c = _buffer[i + j];
					char d = str._buffer[j];
					if (!caseSensitive)
					{
						c = (char)tolower(c);
						d = (char)tolower(d);
					}

					if (skip == NO_POS && c == first)
						skip = i + j - 1;

					if (c != d)
					{
						found = false;
						if (skip != NO_POS)
							i = skip;
						break;
					}
				}
				if (found)
					return i;
			}
		}

		return NO_POS;
	}

	/**
	* @brief : Return index to the last occurrence of a string, or NPOS if not found
	*/
	unsigned FindLast(const tString& str, unsigned startPos = NO_POS, bool caseSensitive = true) const
	{
		if (!str._length || str._length > _length)
			return NO_POS;
		if (startPos > _length - str._length)
			startPos = _length - str._length;

		char first = str._buffer[0];
		if (!caseSensitive)
			first = (char)tolower(first);

		for (unsigned i = startPos; i < _length; --i)
		{
			char c = _buffer[i];
			if (!caseSensitive)
				c = (char)tolower(c);

			if (c == first)
			{
				bool found = true;
				for (unsigned j = 1; j < str._length; ++j)
				{
					c = _buffer[i + j];
					char d = str._buffer[j];
					if (!caseSensitive)
					{
						c = (char)tolower(c);
						d = (char)tolower(d);
					}

					if (c != d)
					{
						found = false;
						break;
					}
				}
				if (found)
					return i;
			}
		}

		return NO_POS;
	}
	/**
	* @brief : Return index to the last occurrence of a character, or NO_POS if not found
	*/
	unsigned FindLast(char c, unsigned startPos = NO_POS, bool caseSensitive = true) const
	{
		if (startPos >= _length)
			startPos = _length - 1;

		if (caseSensitive)
		{
			for (unsigned i = startPos; i < _length; --i)
			{
				if (_buffer[i] == c)
					return i;
			}
		}
		else
		{
			c = (char)tolower(c);
			for (unsigned i = startPos; i < _length; --i)
			{
				if (tolower(_buffer[i]) == c)
					return i;
			}
		}

		return NO_POS;
	}
	/**
	* @brief : Return whether starts with a string
	*/
	bool StartsWith(const tString& str, bool caseSensitive = true) const
	{
		return Find(str, 0, caseSensitive) == 0;
	}

	/**
	* @brief : Return whether ends with a string
	*/
	bool EndsWith(const tString& str, bool caseSensitive = true) const
	{
		unsigned pos = FindLast(str, Length() - 1, caseSensitive);
		return pos != NO_POS && pos == Length() - str.Length();
	}

	/**
	* @brief : Return C str
	*/
	const char* CStr() const
	{
		return _buffer;
	}
	/**
	* @brief : Return length
	*/
	unsigned Length() const
	{
		return _length;
	}
	/**
	* @brief : Return buffer capacity
	*/
	unsigned Capacity() const
	{
		return _capacity;
	}
	/**
	* @brief : Return whether the string is Empty
	*/
	bool Empty() const
	{
		return _length == 0;
	}
	/**
	* @brief : Return comparison result with a string
	*/
	int Compare(const tString& str, bool caseSensitive = true) const
	{
		return Compare(CStr(), str.CStr(), caseSensitive);
	}
	/**
	* @brief : Return comparison result with a C string
	*/
	int Compare(const char* str, bool caseSensitive = true) const
	{
		return Compare(CStr(), str, caseSensitive);
	}
	/**
	* @brief : Return whether contains a specific occurrence of a string
	*/
	bool Contains(const tString& str, bool caseSensitive = true) const
	{
		return Find(str, 0, caseSensitive) != NO_POS;
	}
	/**
	* @brief : Return whether contains a specific char
	*/
	bool Contains(char c, bool caseSensitive = true) const
	{
		return Find(c, 0, caseSensitive) != NO_POS;
	}
	/**
	* @brief : Construct UTF8 content from Latin1
	*/
	void SetUTF8FromLatin1(const char* str)
	{
		char tc[7];

		Clear();
		if (!str)
			return;

		while (*str)
		{
			char* dest = tc;
			EncodeUTF8(dest, (unsigned)*str++);
			*dest = 0;
			Append(tc);
		}
	}
	/**
	* @brief : Constructing UTF 8 content with wide char
	*/
	void SetUTF8FromWChar(const wchar_t* str)
	{
		char temp[7];

		Clear();

		if (!str)
			return;

#ifdef _WIN32
		while (*str)
		{
			unsigned unicodeChar = DecodeUTF16(str);
			char* dest = temp;
			EncodeUTF8(dest, unicodeChar);
			*dest = 0;
			Append(temp);
		}
#else
		while (*str)
		{
			char* dest = temp;
			EncodeUTF8(dest, (unsigned)*str++);
			*dest = 0;
			Append(temp);
		}
#endif
	}
	/**
	* @brief : Calculate number of characters in UTF8 content
	*/
	unsigned LengthUTF8() const
	{
		unsigned ret = 0;

		const char* src = _buffer;
		if (!src)
			return ret;
		const char* end = _buffer + _length;

		while (src < end)
		{
			DecodeUTF8(src);
			++ret;
		}

		return ret;
	}
	/**
	* @brief : Return byte offset to char in UTF8 content
	*/
	unsigned ByteOffsetUTF8(unsigned index) const
	{
		unsigned byteOffset = 0;
		unsigned utfPos = 0;

		while (utfPos < index && byteOffset < _length)
		{
			NextUTF8Char(byteOffset);
			++utfPos;
		}

		return byteOffset;
	}
	/**
	* @brief : Return next Unicode character from UTF8 content and increase byte offset
	*/
	unsigned NextUTF8Char(unsigned& byteOffset) const
	{
		if (!_buffer)
			return 0;

		const char* src = _buffer + byteOffset;
		unsigned ret = DecodeUTF8(src);
		byteOffset = (unsigned)(src - _buffer);

		return ret;
	}
	/**
	* @brief : Return Unicode character at index from UTF8 content
	*/
	unsigned AtUTF8(unsigned index) const
	{
		unsigned byteOffset = ByteOffsetUTF8(index);
		return NextUTF8Char(byteOffset);
	}

	/**
	* @brief : Replace Unicode character at index from UTF8 content
	*/
	void ReplaceUTF8(unsigned index, unsigned unicodeChar)
	{
		unsigned utfPos = 0;
		unsigned byteOffset = 0;

		while (utfPos < index && byteOffset < _length)
		{
			NextUTF8Char(byteOffset);
			++utfPos;
		}

		if (utfPos < index)
			return;

		unsigned beginCharPos = byteOffset;
		NextUTF8Char(byteOffset);

		char temp[7];
		char* dest = temp;
		EncodeUTF8(dest, unicodeChar);
		*dest = 0;

		Replace(beginCharPos, byteOffset - beginCharPos, temp, (unsigned)(dest - temp));
	}
	/**
	* @brief : Append Unicode character at the end as UTF8
	*/
	tString& AppendUTF8(unsigned unicodeChar)
	{
		char temp[7];
		char* dest = temp;
		EncodeUTF8(dest, unicodeChar);
		*dest = 0;
		return Append(temp);
	}
	/**
	* @brief : Return a UTF8 substring from position to end
	*/
	tString SubstringUTF8(unsigned pos) const
	{
		unsigned utf8Length = LengthUTF8();
		unsigned byteOffset = ByteOffsetUTF8(pos);
		tString ret;

		while (pos < utf8Length)
		{
			ret.AppendUTF8(NextUTF8Char(byteOffset));
			++pos;
		}

		return ret;
	}
	/**
	* @brief : Return a UTF8 substring with length from position
	*/
	tString SubstringUTF8(unsigned pos, unsigned length) const
	{
		unsigned utf8Length = LengthUTF8();
		unsigned byteOffset = ByteOffsetUTF8(pos);
		unsigned endPos = pos + length;
		tString ret;

		while (pos < endPos && pos < utf8Length)
		{
			ret.AppendUTF8(NextUTF8Char(byteOffset));
			++pos;
		}

		return ret;
	}
	/**
	* @brief : Return hash value for tHashSet & tHashMap
	*/
	unsigned ToHash() const
	{
		unsigned hash = 0;
		const char* ptr = _buffer;
		while (*ptr)
		{
			hash = *ptr + (hash << 6u) + (hash << 16u) - hash;
			++ptr;
		}

		return hash;
	}
	/**
	* @brief : Return substrings split by a separator char. By default don't return Empty strings
	*/
	static std::vector<tString> Split(const char* str, char separator, bool keepEmptyStrings = false)
	{
		std::vector<tString> ret;
		const char* strEnd = str + tString::CStrLength(str);

		for (const char* splitEnd = str; splitEnd != strEnd; ++splitEnd)
		{
			if (*splitEnd == separator)
			{
				const ptrdiff_t splitLen = splitEnd - str;
				if (splitLen > 0 || keepEmptyStrings)
					ret.push_back(tString(str, splitLen));
				str = splitEnd + 1;
			}
		}

		const ptrdiff_t splitLen = strEnd - str;
		if (splitLen > 0 || keepEmptyStrings)
			ret.push_back(tString(str, splitLen));

		return ret;
	}
	/**
	* @brief : Return a string by joining substrings with a 'glue' string
	*/
	static tString Joined(const std::vector<tString>& subStrings, const tString& glue)
	{
		if (subStrings.empty())
			return tString();

		tString joinedString(subStrings[0]);
		for (unsigned i = 1; i < subStrings.size(); ++i)
			joinedString.Append(glue).Append(subStrings[i]);

		return joinedString;
	}
	/**
	* @brief : Encode Unicode character to UTF8. Pointer will be incremented
	*/
	static void EncodeUTF8(char*& dest, unsigned unicodeChar)
	{
		if (unicodeChar < 0x80)
			*dest++ = unicodeChar;
		else if (unicodeChar < 0x800)
		{
			dest[0] = (char)(0xc0u | ((unicodeChar >> 6u) & 0x1fu));
			dest[1] = (char)(0x80u | (unicodeChar & 0x3fu));
			dest += 2;
		}
		else if (unicodeChar < 0x10000)
		{
			dest[0] = (char)(0xe0u | ((unicodeChar >> 12u) & 0xfu));
			dest[1] = (char)(0x80u | ((unicodeChar >> 6u) & 0x3fu));
			dest[2] = (char)(0x80u | (unicodeChar & 0x3fu));
			dest += 3;
		}
		else if (unicodeChar < 0x200000)
		{
			dest[0] = (char)(0xf0u | ((unicodeChar >> 18u) & 0x7u));
			dest[1] = (char)(0x80u | ((unicodeChar >> 12u) & 0x3fu));
			dest[2] = (char)(0x80u | ((unicodeChar >> 6u) & 0x3fu));
			dest[3] = (char)(0x80u | (unicodeChar & 0x3fu));
			dest += 4;
		}
		else if (unicodeChar < 0x4000000)
		{
			dest[0] = (char)(0xf8u | ((unicodeChar >> 24u) & 0x3u));
			dest[1] = (char)(0x80u | ((unicodeChar >> 18u) & 0x3fu));
			dest[2] = (char)(0x80u | ((unicodeChar >> 12u) & 0x3fu));
			dest[3] = (char)(0x80u | ((unicodeChar >> 6u) & 0x3fu));
			dest[4] = (char)(0x80u | (unicodeChar & 0x3fu));
			dest += 5;
		}
		else
		{
			dest[0] = (char)(0xfcu | ((unicodeChar >> 30u) & 0x1u));
			dest[1] = (char)(0x80u | ((unicodeChar >> 24u) & 0x3fu));
			dest[2] = (char)(0x80u | ((unicodeChar >> 18u) & 0x3fu));
			dest[3] = (char)(0x80u | ((unicodeChar >> 12u) & 0x3fu));
			dest[4] = (char)(0x80u | ((unicodeChar >> 6u) & 0x3fu));
			dest[5] = (char)(0x80u | (unicodeChar & 0x3fu));
			dest += 6;
		}
	}
	/**
	* @brief : Decode Unicode character from UTF8. Pointer will be incremented
	*/
	static unsigned DecodeUTF8(const char*& src)
	{
		if (src == nullptr)
			return 0;

		unsigned char char1 = *src++;

		// Check if we are in the middle of a UTF8 character
		if (char1 >= 0x80 && char1 < 0xc0)
		{
			while ((unsigned char)*src >= 0x80 && (unsigned char)*src < 0xc0)
				++src;
			return '?';
		}

		if (char1 < 0x80)
			return char1;
		else if (char1 < 0xe0)
		{
			unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
			return (unsigned)((char2 & 0x3fu) | ((char1 & 0x1fu) << 6u));
		}
		else if (char1 < 0xf0)
		{
			unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
			return (unsigned)((char3 & 0x3fu) | ((char2 & 0x3fu) << 6u) | ((char1 & 0xfu) << 12u));
		}
		else if (char1 < 0xf8)
		{
			unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
			return (unsigned)((char4 & 0x3fu) | ((char3 & 0x3fu) << 6u) | ((char2 & 0x3fu) << 12u) | ((char1 & 0x7u) << 18u));
		}
		else if (char1 < 0xfc)
		{
			unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
			return (unsigned)((char5 & 0x3fu) | ((char4 & 0x3fu) << 6u) | ((char3 & 0x3fu) << 12u) | ((char2 & 0x3fu) << 18u) |
				((char1 & 0x3u) << 24u));
		}
		else
		{
			unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
			unsigned char char6 = GET_NEXT_CONTINUATION_BYTE(src);
			return (unsigned)((char6 & 0x3fu) | ((char5 & 0x3fu) << 6u) | ((char4 & 0x3fu) << 12u) | ((char3 & 0x3fu) << 18u) |
				((char2 & 0x3fu) << 24u) | ((char1 & 0x1u) << 30u));
		}
	}
#ifdef _WIN32
	/**
	* @brief : Encode Unicode character to UTF16. Pointer will be incremented
	*/
	static void EncodeUTF16(wchar_t*& dest, unsigned unicodeChar)
	{
		if (unicodeChar < 0x10000)
			*dest++ = unicodeChar;
		else
		{
			unicodeChar -= 0x10000;
			*dest++ = 0xd800 | ((unicodeChar >> 10) & 0x3ff);
			*dest++ = 0xdc00 | (unicodeChar & 0x3ff);
		}
	}
	/**
	* @brief : Decode Unicode character from UTF16. Pointer will be incremented
	*/
	static unsigned DecodeUTF16(const wchar_t*& src)
	{
		if (src == nullptr)
			return 0;

		unsigned short word1 = *src++;

		// Check if we are at a low surrogate
		if (word1 >= 0xdc00 && word1 < 0xe000)
		{
			while (*src >= 0xdc00 && *src < 0xe000)
				++src;
			return '?';
		}

		if (word1 < 0xd800 || word1 >= 0xe000)
			return word1;
		else
		{
			unsigned short word2 = *src++;
			if (word2 < 0xdc00 || word2 >= 0xe000)
			{
				--src;
				return '?';
			}
			else
				return (((word1 & 0x3ff) << 10) | (word2 & 0x3ff)) + 0x10000;
		}
	}
#endif
	/**
	* @brief : Return length of a C string
	*/
	static unsigned CStrLength(const char* str) { return str ? (unsigned)strlen(str) : 0; }

	/**
	* @brief : Append to string using formatting
	*/
	tString& AppendWithFormat(const char* formatString, ...)
	{
		va_list args;
		va_start(args, formatString);
		AppendWithFormatArgs(formatString, args);
		va_end(args);
		return *this;
	}
	/**
	* @brief : Append to string using variable arguments
	*/
	tString& AppendWithFormatArgs(const char* formatString, va_list args)
	{
		int pos = 0, lastPos = 0;
		auto length = (int)strlen(formatString);

		while (true)
		{
			// Scan the format string and Find %a argument where a is one of d, f, s ...
			while (pos < length && formatString[pos] != '%') pos++;
			Append(formatString + lastPos, (unsigned)(pos - lastPos));
			if (pos >= length)
				return *this;

			char format = formatString[pos + 1];
			pos += 2;
			lastPos = pos;

			switch (format)
			{
				// Integer
			case 'd':
			case 'i':
			{
				int arg = va_arg(args, int);
				Append(tString(arg));
				break;
			}

			// Unsigned
			case 'u':
			{
				unsigned arg = va_arg(args, unsigned);
				Append(tString(arg));
				break;
			}

			// Unsigned long
			case 'l':
			{
				unsigned long arg = va_arg(args, unsigned long);
				Append(tString(arg));
				break;
			}

			// Real
			case 'f':
			{
				double arg = va_arg(args, double);
				Append(tString(arg));
				break;
			}

			// Character
			case 'c':
			{
				int arg = va_arg(args, int);
				Append((char)arg);
				break;
			}

			// C string
			case 's':
			{
				char* arg = va_arg(args, char*);
				Append(arg);
				break;
			}

			// Hex
			case 'x':
			{
				char buf[BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf(buf, "%x", arg);
				Append(buf, (unsigned)arglen);
				break;
			}

			// Pointer
			case 'p':
			{
				char buf[BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf(buf, "%p", reinterpret_cast<void*>(arg));
				Append(buf, (unsigned)arglen);
				break;
			}

			case '%':
			{
				Append("%", 1);
				break;
			}

			default:
				printf("Unsupported format specifier: '%c'", format);
				break;
			}
		}
	}
	/**
	* @brief : Compare two C strings
	*/
	static int Compare(const char* lhs, const char* rhs, bool caseSensitive)
	{
		if (!lhs || !rhs)
			return lhs ? 1 : (rhs ? -1 : 0);

		if (caseSensitive)
			return strcmp(lhs, rhs);
		else
		{
			for (;;)
			{
				auto l = (char)tolower(*lhs);
				auto r = (char)tolower(*rhs);
				if (!l || !r)
					return l ? 1 : (r ? -1 : 0);
				if (l < r)
					return -1;
				if (l > r)
					return 1;

				++lhs;
				++rhs;
			}
		}
	}
	/// Position for "not found."
	static const unsigned NO_POS = 0xffffffff;
	/// Initial dynamic allocation size.
	static const unsigned MIN_CAPACITY = 8;
	/// Empty string.
	static const tString EMPTY;

	/// string length
	unsigned _length;
	/// capacity, zero if buffer not allocated
	unsigned _capacity;
	/// string buffer, point to &endZero if buffer is not allocated
	char* _buffer;

	/// end zero for Empty strings
	char endZero{};
private:
	/**
	* @brief : Move a range of characters within the string
	*/
	void MoveRange(unsigned dest, unsigned src, unsigned count)
	{
		if (count)
			memmove(_buffer + dest, _buffer + src, count);
	}
	/**
	* @brief : Copy chars from one buffer to another.
	*/
	static void CopyChars(char* dest, const char* src, unsigned count)
	{
#ifdef _MSC_VER
		if (count)
			memcpy(dest, src, count);
#else
		char* end = dest + count;
		while (dest != end)
		{
			*dest = *src;
			++dest;
			++src;
		}
#endif
	}

};

std::istream& operator >>(std::istream &in, tString& rhs);

std::ostream& operator <<(std::ostream& out, tString& rhs);

std::wistream& operator >>(std::wistream &in, tWString& rhs);

std::wostream& operator <<(std::wostream& out, tWString& rhs);
/**
* @brief : Add a string to a C string
*/
inline tString operator +(const char* lhs, const tString& rhs)
{
	tString ret(lhs);
	ret += rhs;
	return ret;
}

/**
* @brief : Add a string to a wide char C string
*/
inline tString operator +(const wchar_t* lhs, const tString& rhs)
{
	tString ret(lhs);
	ret += rhs;
	return ret;
}


template <> void Swap<tString>(tString& rhs, tString& lfs)
{
	rhs.Swap(lfs);
}

// ToString NARROW CONVERSIONS
inline tString ToString(int value)
{	// convert int to string
	return tString(value);
}

inline tString ToString(unsigned int value)
{	// convert unsigned int to string
	return tString(value);
}

inline tString ToString(long value)
{	// convert long to string
	return tString(value);
}

inline tString ToString(unsigned long value)
{	// convert unsigned long to string
	return tString(value);
}

inline tString ToString(long long value)
{	// convert long long to string
	return tString(value);
}

inline tString ToString(unsigned long long value)
{	// convert unsigned long long to string
	return tString(value);
}

inline tString ToString(float value)
{	// convert float to string
	return tString(value);
}

inline tString ToString(double value)
{	// convert double to string
	return tString(value);
}

inline tString ToString(long double value)
{	// convert long double to string
	return tString(value);
}
inline tString ToString(bool value)
{
	return tString(value);
}

inline tString ToString(char value)
{
	return tString(value);
}
template <typename _Sty> inline tString ToString(const _Sty& value)
{
	return tString(value);
}

}

namespace std {
	template<> struct hash<KhSTL::tString> {
	public:
		size_t operator()(const KhSTL::tString &lhs) const
		{
			unsigned hash = 0;
			const char* ptr = lhs._buffer;
			while (*ptr)
			{
				hash = *ptr + (hash << 6u) + (hash << 16u) - hash;
				++ptr;
			}

			return hash;
		}
	};
}
#endif //!KH_STL_TYPE_STRING_H_