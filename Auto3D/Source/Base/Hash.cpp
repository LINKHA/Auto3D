#include "Hash.h"
#include "Swap.h"

#include <cassert>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void HashBase::Swap(HashBase& hash)
{
    Auto3D::Swap(ptrs, hash.ptrs);
    Auto3D::Swap(allocator, hash.allocator);
}

void HashBase::AllocateBuckets(size_t size, size_t numBuckets)
{
    assert(numBuckets >= MIN_BUCKETS);

    // Remember old head & tail pointers
    HashNodeBase* head = Head();
    HashNodeBase* tail = Tail();
    delete[] ptrs;

    HashNodeBase** newPtrs = new HashNodeBase*[numBuckets + 4];
    size_t* data = reinterpret_cast<size_t*>(newPtrs);
    data[0] = size;
    data[1] = numBuckets;
    newPtrs[2] = head;
    newPtrs[3] = tail;
    ptrs = newPtrs;
    
    ResetPtrs();
}

void HashBase::ResetPtrs()
{
    if (ptrs)
    {
        size_t numBuckets = NumBuckets();
        HashNodeBase** data = Ptrs();
        for (size_t i = 0; i < numBuckets; ++i)
            data[i] = nullptr;
    }
}

template<> void Swap<HashBase>(HashBase& first, HashBase& second)
{
    first.Swap(second);
}

}
