#include "Hash.h"
#include "Swap.h"

#include <cassert>

#include "Debug/DebugNew.h"

namespace Auto3D
{

void FHashBase::Swap(FHashBase& hash)
{
    Auto3D::Swap(_ptrs, hash._ptrs);
    Auto3D::Swap(_allocator, hash._allocator);
}

void FHashBase::AllocateBuckets(size_t _size, size_t numBuckets)
{
    assert(numBuckets >= MIN_BUCKETS);

    // Remember old head & tail pointers
    FHashNodeBase* head = Head();
    FHashNodeBase* tail = Tail();
    delete[] _ptrs;

    FHashNodeBase** newPtrs = new FHashNodeBase*[numBuckets + 4];
    size_t* _data = reinterpret_cast<size_t*>(newPtrs);
    _data[0] = _size;
    _data[1] = numBuckets;
    newPtrs[2] = head;
    newPtrs[3] = tail;
    _ptrs = newPtrs;
    
    ResetPtrs();
}

void FHashBase::ResetPtrs()
{
    if (_ptrs)
    {
        size_t numBuckets = NumBuckets();
        FHashNodeBase** _data = Ptrs();
        for (size_t i = 0; i < numBuckets; ++i)
            _data[i] = nullptr;
    }
}

template<> void Swap<FHashBase>(FHashBase& first, FHashBase& second)
{
    first.Swap(second);
}

}
