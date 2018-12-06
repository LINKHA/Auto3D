#ifndef KH_STL_TYPE_ALGORITHM_H_
#define KH_STL_TYPE_ALGORITHM_H_
#include "../Utility/TypeIterator.h"

namespace KhSTL {


static const int QUICKSORT_THRESHOLD = 16;

/**
* @brief : Perform insertion sort on an array
*/
template <class T> static void InsertionSort(tIterator<T> begin, tIterator<T> end)
{
	for (tIterator<T> i = begin + 1; i < end; ++i)
	{
		T temp = *i;
		tIterator<T> j = i;
		while (j > begin && temp < *(j - 1))
		{
			*j = *(j - 1);
			--j;
		}
		*j = temp;
	}
}
/**
* @brief : Perform insertion sort on an array using a compare function
*/
template <class T, class U> static void InsertionSort(tIterator<T> begin, tIterator<T> end, U compare)
{
	for (tIterator<T> i = begin + 1; i < end; ++i)
	{
		T temp = *i;
		tIterator<T> j = i;
		while (j > begin && compare(temp, *(j - 1)))
		{
			*j = *(j - 1);
			--j;
		}
		*j = temp;
	}
}
/**
* @brief : Perform quick sort initial pass on an array. Does not sort fully
*/
template <class T> static void InitialQuickSort(tIterator<T> begin, tIterator<T> end)
{
	while (end - begin > QUICKSORT_THRESHOLD)
	{
		// Choose the pivot by median
		tIterator<T> pivot = begin + ((end - begin) / 2);
		if (*begin < *pivot && *(end - 1) < *begin)
			pivot = begin;
		else if (*(end - 1) < *pivot && *begin < *(end - 1))
			pivot = end - 1;

		// Partition and sort recursively
		tIterator<T> i = begin - 1;
		tIterator<T> j = end;
		T pivotValue = *pivot;
		for (;;)
		{
			while (pivotValue < *(--j));
			while (*(++i) < pivotValue);
			if (i < j)
				Swap(*i, *j);
			else
				break;
		}

		InitialQuickSort(begin, j + 1);
		begin = j + 1;
	}
}
/**
* @brief : Perform quick sort initial pass on an array using a
*			compare function. Does not sort fully
*/
template <class T, class U> static void InitialQuickSort(tIterator<T> begin, tIterator<T> end, U compare)
{
	while (end - begin > QUICKSORT_THRESHOLD)
	{
		// Choose the pivot by median
		tIterator<T> pivot = begin + ((end - begin) / 2);
		if (compare(*begin, *pivot) && compare(*(end - 1), *begin))
			pivot = begin;
		else if (compare(*(end - 1), *pivot) && compare(*begin, *(end - 1)))
			pivot = end - 1;

		// Partition and sort recursively
		tIterator<T> i = begin - 1;
		tIterator<T> j = end;
		T pivotValue = *pivot;
		for (;;)
		{
			while (compare(pivotValue, *(--j)));
			while (compare(*(++i), pivotValue));
			if (i < j)
				Swap(*i, *j);
			else
				break;
		}

		InitialQuickSort(begin, j + 1, compare);
		begin = j + 1;
	}
}
/**
* @brief : Sort in ascending order using quicksort for initial passes,
*			then an insertion sort to finalize
*/
template <class T> static void Sort(tIterator<T> begin, tIterator<T> end)
{
	InitialQuickSort(begin, end);
	InsertionSort(begin, end);
}
/**
* @brief : Sort in ascending order using quicksort for initial passes,
*			then an insertion sort to finalize, using a compare function
*/
template <class T, class U> static void Sort(tIterator<T> begin, tIterator<T> end, U compare)
{
	InitialQuickSort(begin, end, compare);
	InsertionSort(begin, end, compare);
}
/**
* Swap two values.
*/
template <typename _Ty> static inline void Swap(_Ty& rhs, _Ty& lfs)
{
	_Ty temp(rhs);
	rhs = lfs;
	lfs = temp;
}


//template <> void Swap<tString>(tString& rhs, tString& lfs);
//template <> void Swap<tVectorValue>(tVectorValue& rhs, tVectorValue& lfs);
//template <> void Swap<tHashBase>(tHashBase& rhs, tHashBase& lfs);

//template <typename _Ty, typename _Alloc> 
//	void Swap<tListValue<_Ty, _Alloc>>(
//		tListValue<_Ty, _Alloc>& rhs,
//		tListValue<_Ty, _Alloc>& lfs);

//
//template<> void Swap(tString& rhs, tString& lfs)
//{
//	rhs.Swap(lfs);
//}
//template<> void Swap(tVectorValue& rhs, tVectorValue& lfs)
//{
//	rhs.Swap(lfs);
//}
//template <> void Swap(tHashBase& rhs, tHashBase& lfs)
//{
//	rhs.Swap(lfs);
//}


}
#endif //!KH_STL_TYPE_ALGORITHM_H_s