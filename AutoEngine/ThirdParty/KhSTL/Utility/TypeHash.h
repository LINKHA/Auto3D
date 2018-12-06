#ifndef KH_STL_TYPE_HASH_H_
#define KH_STL_TYPE_HASH_H_
#include <cstddef>

namespace KhSTL
{
/**
* @brief : Pointer hash function
*/
template <typename _Ty> unsigned MakeHash(_Ty* value)
{
	return (unsigned)((size_t)value / sizeof(_Ty));
}
/**
* @brief : Const pointer hash function
*/
template <typename _Ty> unsigned MakeHash(const _Ty* value)
{
	return (unsigned)((size_t)value / sizeof(_Ty));
}
/**
* @brief : Generic hash function
*/
template <typename _Ty> unsigned MakeHash(const _Ty& value)
{
	return value.ToHash();
}
/**
* @brief : Void pointer hash function
*/
template <> inline unsigned MakeHash(void* value)
{
	return (unsigned)(size_t)value;
}
/**
* @brief : Const void pointer hash function
*/
template <> inline unsigned MakeHash(const void* value)
{
	return (unsigned)(size_t)value;
}
/**
* @brief : Long long hash function
*/
template <> inline unsigned MakeHash(const long long& value)
{
	return (unsigned)((value >> 32u) | (value & 0xffffffffu));
}
/**
* @brief : Unsigned long long hash function
*/
template <> inline unsigned MakeHash(const unsigned long long& value)
{
	return (unsigned)((value >> 32u) | (value & 0xffffffffu));
}
/**
* @brief : Int hash function
*/
template <> inline unsigned MakeHash(const int& value)
{
	return (unsigned)value;
}
/**
* @brief : Unsigned hash function
*/
template <> inline unsigned MakeHash(const unsigned& value)
{
	return value;
}
/**
* @brief : Short hash function
*/
template <> inline unsigned MakeHash(const short& value)
{
	return (unsigned)value;
}
/**
* @brief : Unsigned short hash function
*/
template <> inline unsigned MakeHash(const unsigned short& value)
{
	return value;
}
/**
* @brief : Char hash function
*/
template <> inline unsigned MakeHash(const char& value)
{
	return (unsigned)value;
}
/**
* @brief : Unsigned char hash function
*/
template <> inline unsigned MakeHash(const unsigned char& value)
{
	return value;
}


}
#endif //!KH_STL_TYPE_HASH_H_