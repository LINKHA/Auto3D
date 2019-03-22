#pragma once

#include "Iterator.h"
#include "Swap.h"

namespace Auto3D
{

static const int QUICKSORT_THRESHOLD = 16;

// Based on Comparison of several sorting algorithms by Juha Nieminen
// http://warp.povusers.org/SortComparison/

/// Perform insertion sort on an array.
template <typename _Ty> void InsertionSort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end)
{
    for (RandomAccessIterator<_Ty> i = begin + 1; i < end; ++i)
    {
        _Ty temp = *i;
        RandomAccessIterator<_Ty> j = i;
        while (j > begin && temp < *(j - 1))
        {
            *j = *(j - 1);
            --j;
        }
        *j = temp;
    }
}

/// Perform insertion sort on an array using a compare function.
template <typename _Ty, typename _Oth> void InsertionSort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end, _Oth compare)
{
    for (RandomAccessIterator<_Ty> i = begin + 1; i < end; ++i)
    {
        _Ty temp = *i;
        RandomAccessIterator<_Ty> j = i;
        while (j > begin && compare(temp, *(j - 1)))
        {
            *j = *(j - 1);
            --j;
        }
        *j = temp;
    }
}

/// Perform quick sort initial pass on an array. Does not sort fully.
template <typename _Ty> void InitialQuickSort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end)
{
    while (end - begin > QUICKSORT_THRESHOLD)
    {
        // Choose the pivot by median
        RandomAccessIterator<_Ty> pivot = begin + ((end - begin) / 2);
        if (*begin < *pivot && *(end - 1) < *begin)
            pivot = begin;
        else if (*(end - 1) < *pivot && *begin < *(end - 1))
            pivot = end - 1;
        
        // Partition and sort recursively
        RandomAccessIterator<_Ty> i = begin - 1;
        RandomAccessIterator<_Ty> j = end;
        _Ty pivotValue = *pivot;
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

/// Perform quick sort initial pass on an array using a compare function. Does not sort fully.
template <typename _Ty, typename _Fun> void InitialQuickSort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end, _Fun compare)
{
    while (end - begin > QUICKSORT_THRESHOLD)
    {
        // Choose the pivot by median
        RandomAccessIterator<_Ty> pivot = begin + ((end - begin) / 2);
        if (compare(*begin, *pivot) && compare(*(end - 1), *begin))
            pivot = begin;
        else if (compare(*(end - 1), *pivot) && compare(*begin, *(end - 1)))
            pivot = end - 1;
        
        // Partition and sort recursively
        RandomAccessIterator<_Ty> i = begin - 1;
        RandomAccessIterator<_Ty> j = end;
        _Ty pivotValue = *pivot;
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

/// Sort in ascending order using quicksort for initial passes, then an insertion sort to finalize.
template <typename _Ty> void Sort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end)
{
    InitialQuickSort(begin, end);
    InsertionSort(begin, end);
}

/// Sort in ascending order using quicksort for initial passes, then an insertion sort to finalize, using a compare function.
template <typename _Ty, typename _Fun> void Sort(RandomAccessIterator<_Ty> begin, RandomAccessIterator<_Ty> end, _Fun compare)
{
    InitialQuickSort(begin, end, compare);
    InsertionSort(begin, end, compare);
}

}
