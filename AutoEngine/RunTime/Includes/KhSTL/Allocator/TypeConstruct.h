#pragma once

template<class T1, class  T2>
inline void construct(T1* p, const T2& value)
{
	new (p)T1(value);
}

template<typename T>
inline void destroy(T* pointer)
{
	pointer->~T();
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	for (; first < last; ++first)
		destroy(&*first);
}
