#pragma once

#include "Allocator.h"
#include "Hash.h"
#include "Sort.h"
#include "Vector.h"

namespace Auto3D
{

/// Hash set template class.
template <typename _Ty> class THashSet : public FHashBase
{
public:
    /// Hash set node.
    struct ANode : public FHashNodeBase
    {
        /// Construct undefined.
        ANode()
        {
        }
        
        /// Construct with _key.
        ANode(const _Ty& rhs) :
            _key(rhs)
        {
        }
        
        /// Key.
        _Ty _key;
        
        /// Return next node.
        ANode* Next() const { return static_cast<ANode*>(_next); }
        /// Return previous node.
        ANode* Prev() const { return static_cast<ANode*>(_prev); }
        /// Return next node in the bucket.
        ANode* Down() const { return static_cast<ANode*>(_down); }
    };
    
    /// Hash set node iterator.
    struct Iterator : public FHashIteratorBase
    {
        /// Construct.
        Iterator()
        {
        }
        
        /// Construct with a node pointer.
        Iterator(ANode* rhs) :
            FHashIteratorBase(rhs)
        {
        }
        
        /// Preincrement the pointer.
        Iterator& operator ++ () { GotoNext(); return *this; }
        /// Postincrement the pointer.
        Iterator operator ++ (int) { Iterator it = *this; GotoNext(); return it; }
        /// Predecrement the pointer.
        Iterator& operator -- () { GotoPrev(); return *this; }
        /// Postdecrement the pointer.
        Iterator operator -- (int) { Iterator it = *this; GotoPrev(); return it; }
        
        /// Point to the _key.
        const _Ty* operator -> () const { return &(static_cast<ANode*>(_ptr))->_key; }
        /// Dereference the _key.
        const _Ty& operator * () const { return (static_cast<ANode*>(_ptr))->_key; }
    };
    
    /// Hash set node const iterator.
    struct ConstIterator : public FHashIteratorBase
    {
        /// Construct.
        ConstIterator()
        {
        }
        
        /// Construct with a node pointer.
        ConstIterator(ANode* rhs) :
            FHashIteratorBase(rhs)
        {
        }
        
        /// Construct from a non-const iterator.
        ConstIterator(const Iterator& rhs) :
            FHashIteratorBase(rhs.ptr)
        {
        }
        
        /// Assign from a non-const iterator.
        ConstIterator& operator = (const Iterator& rhs) { _ptr = rhs.ptr; return *this; }
        /// Preincrement the pointer.
        ConstIterator& operator ++ () { GotoNext(); return *this; }
        /// Postincrement the pointer.
        ConstIterator operator ++ (int) { ConstIterator it = *this; GotoNext(); return it; }
        /// Predecrement the pointer.
        ConstIterator& operator -- () { GotoPrev(); return *this; }
        /// Postdecrement the pointer.
        ConstIterator operator -- (int) { ConstIterator it = *this; GotoPrev(); return it; }
        
        /// Point to the _key.
        const _Ty* operator -> () const { return &(static_cast<ANode*>(_ptr))->key; }
        /// Dereference the _key.
        const _Ty& operator * () const { return (static_cast<ANode*>(_ptr))->key; }
    };
    
    /// Construct empty.
    THashSet()
    {
    }
    
    /// Construct from another hash set.
    THashSet(const THashSet<_Ty>& set)
    {
        Initialize(set.NumBuckets(), set.Size() + 1);
        *this = set;
    }
    
    /// Destruct.
    ~THashSet()
    {
        if (_ptrs && _allocator)
        {
            Clear();
            FreeNode(Tail());
            AllocatorUninitialize(_allocator);
        }
    }
    
    /// Assign a hash set.
    THashSet& operator = (const THashSet<_Ty>& rhs)
    {
        if (&rhs != this)
        {
            Clear();
            Insert(rhs);
        }
        return *this;
    }
    
    /// Add-assign a value.
    THashSet& operator += (const _Ty& rhs)
    {
        Insert(rhs);
        return *this;
    }
    
    /// Add-assign a hash set.
    THashSet& operator += (const THashSet<_Ty>& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /// Test for equality with another hash set.
    bool operator == (const THashSet<_Ty>& rhs) const
    {
        if (rhs.Size() != Size())
            return false;
        
        for (ConstIterator it = Begin(); it != End(); ++it)
        {
            if (!rhs.Contains(*it))
                return false;
        }
        
        return true;
    }
    
    /// Test for inequality with another hash set.
    bool operator != (const THashSet<_Ty>& rhs) const { return !(*this == rhs); }

    /// Insert a _key. Return an iterator to it.
    Iterator Insert(const _Ty& key)
    {
        unsigned hashKey = Hash(key);
        
        ANode* existing = FindNode(key, hashKey);
        if (existing)
            return Iterator(existing);
        
        ANode* newNode = InsertNode(Tail(), key);
        newNode->_down = Ptrs()[hashKey];
        Ptrs()[hashKey] = newNode;
        
        // Rehash if the maximum load factor has been exceeded
        if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
        {
            AllocateBuckets(Size(), NumBuckets() << 1);
            Rehash();
        }
        
        return Iterator(newNode);
    }
    
    /// Insert a set.
    void Insert(const THashSet<_Ty>& set)
    {
        for (ConstIterator it = set.Begin(); it != set.End(); ++it)
            Insert(*it);
    }
    
    /// Insert a _key by iterator. Return iterator to the value.
    Iterator Insert(const ConstIterator& it)
    {
        return Iterator(InsertNode(*it));
    }
    
    /// Erase a _key. Return true if was found.
    bool Erase(const _Ty& key)
    {
        if (!_ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        
        ANode* previous;
        ANode* node = FindNode(key, hashKey, previous);
        if (!node)
            return false;
        
        if (previous)
            previous->down = node->down;
        else
            Ptrs()[hashKey] = node->down;
        
        EraseNode(node);
        return true;
    }
    
    /// Erase a _key by iterator. Return iterator to the next _key.
    Iterator Erase(const Iterator& it)
    {
        if (!_ptrs || !it.ptr)
            return End();
        
        ANode* node = static_cast<ANode*>(it.ptr);
        ANode* next = node->Next();
        
        unsigned hashKey = Hash(node->key);
        
        ANode* previous = nullptr;
        ANode* current = static_cast<ANode*>(Ptrs()[hashKey]);
        while (current && current != node)
        {
            previous = current;
            current = current->Down();
        }
        
        assert(current == node);
        
        if (previous)
            previous->down = node->down;
        else
            Ptrs()[hashKey] = node->down;
        
        EraseNode(node);
        return Iterator(next);
    }
    
    /// Clear the set.
    void Clear()
    {
        if (Size())
        {
            for (Iterator it = Begin(); it != End(); )
                FreeNode(static_cast<ANode*>(it++._ptr));
            
            ANode* tail = Tail();
            tail->_prev = nullptr;
            SetHead(tail);
            SetSize(0);
            ResetPtrs();
        }
    }
    
    /// Sort keys. After sorting the set can be iterated in order until new elements are inserted.
    void Sort()
    {
        size_t numKeys = Size();
        if (!numKeys)
            return;
        
        ANode** ptrs = new ANode*[numKeys];
        ANode* ptr = Head();
        
        for (size_t i = 0; i < numKeys; ++i)
        {
            ptrs[i] = ptr;
            ptr = ptr->Next();
        }
        
        Auto3D::Sort(TRandomAccessIterator<ANode*>(ptrs), TRandomAccessIterator<ANode*>(ptrs + numKeys), CompareNodes);
        
        SetHead(ptrs[0]);
        ptrs[0]->prev = nullptr;
        for (size_t i = 1; i < numKeys; ++i)
        {
            ptrs[i - 1]->next = ptrs[i];
            ptrs[i]->prev = ptrs[i - 1];
        }
        ptrs[numKeys - 1]->next = Tail();
        Tail()->prev = ptrs[numKeys - 1];
        
        delete[] ptrs;
    }
    
    /// Rehash to a specific bucket count, which must be a power of two. Return true on success.
    bool Rehash(size_t numBuckets)
    {
        if (numBuckets == NumBuckets())
            return true;
        if (!numBuckets || numBuckets < Size() / MAX_LOAD_FACTOR)
            return false;
        
        // Check for being power of two
        size_t check = numBuckets;
        while (!(check & 1))
            check >>= 1;
        if (check != 1)
            return false;
        
        AllocateBuckets(Size(), numBuckets);
        Rehash();
        return true;
    }
    
    /// Return iterator to the _key, or end iterator if not found.
    Iterator Find(const _Ty& key)
    {
        if (!_ptrs)
            return End();
        
        unsigned hashKey = Hash(key);
        ANode* node = FindNode(key, hashKey);
        if (node)
            return Iterator(node);
        else
            return End();
    }
    
    /// Return const iterator to the _key, or end iterator if not found.
    ConstIterator Find(const _Ty& key) const
    {
        if (!_ptrs)
            return End();
        
        unsigned hashKey = Hash(key);
        ANode* node = FindNode(key, hashKey);
        if (node)
            return ConstIterator(node);
        else
            return End();
    }
    
    /// Return whether contains a _key.
    bool Contains(const _Ty& key) const
    {
        if (!_ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        return FindNode(key, hashKey) != nullptr;
    }
    
    /// Return iterator to the first element. Is not the lowest value unless the set has been sorted.
    Iterator Begin() { return Iterator(Head()); }
    /// Return const iterator to the first element. Is not the lowest value unless the set has been sorted.
    ConstIterator Begin() const { return ConstIterator(Head()); }
    /// Return iterator to the end.
    Iterator End() { return Iterator(Tail()); }
    /// Return const iterator to the end.
    ConstIterator End() const { return ConstIterator(Tail()); }
    /// Return first _key. Is not the lowest value unless the set has been sorted.
    const _Ty& Front() const { return *Begin(); }
    /// Return last _key.
    const _Ty& Back() const { assert(Size()); return *(--End()); }
    
private:
    /// Return head node with correct type.
    ANode* Head() const { return static_cast<ANode*>(FHashBase::Head()); }
    /// Return tail node with correct type.
    ANode* Tail() const { return static_cast<ANode*>(FHashBase::Tail()); }

    /// Reserve the tail node and initial buckets.
    void Initialize(size_t numBuckets, size_t numNodes)
    {
        AllocateBuckets(0, numBuckets);
        _allocator = AllocatorInitialize(sizeof(ANode), numNodes);
        FHashNodeBase* tail = AllocateNode();
        SetHead(tail);
        SetTail(tail);
    }

    /// Find a node from the buckets.
    ANode* FindNode(const _Ty& key, unsigned hashKey) const
    {
        if (!_ptrs)
            return nullptr;

        ANode* node = static_cast<ANode*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->_key == key)
                return node;
            node = node->Down();
        }
        
        return nullptr;
    }
    
    /// Find a node and the previous node from the buckets.
    ANode* FindNode(const _Ty& key, unsigned hashKey, ANode*& previous) const
    {
        previous = nullptr;
        if (!_ptrs)
            return nullptr;
        
        ANode* node = static_cast<ANode*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->key == key)
                return node;
            previous = node;
            node = node->Down();
        }
        
        return nullptr;
    }
    
    /// Allocate and insert a node into the list. Return the new node.
    ANode* InsertNode(ANode* dest, const _Ty& key)
    {
        // If no pointers yet, allocate with minimum bucket count
        if (!_ptrs)
        {
            Initialize(MIN_BUCKETS, 2);
            dest = Head();
        }

        ANode* newNode = AllocateNode(key);
        ANode* prev = dest->Prev();
        newNode->_next = dest;
        newNode->_prev = prev;
        if (prev)
            prev->_next = newNode;
        dest->_prev = newNode;
        
        // Reassign the head node if necessary
        if (dest == Head())
            SetHead(newNode);
        
        SetSize(Size() + 1);
        
        return newNode;
    }
    
    /// Erase a node from the list. Return pointer to the next element, or to the end if could not erase.
    ANode* EraseNode(ANode* node)
    {
        // The tail node can not be removed
        if (!node || node == Tail())
            return Tail();
        
        ANode* prev = node->Prev();
        ANode* next = node->Next();
        if (prev)
            prev->next = next;
        next->prev = prev;
        
        // Reassign the head node if necessary
        if (node == Head())
            SetHead(next);
        
        FreeNode(node);
        SetSize(Size() - 1);
        
        return next;
    }
    
    /// Allocate a node with optionally specified _key.
    ANode* AllocateNode(const _Ty& key = _Ty())
    {
        ANode* newNode = static_cast<ANode*>(AllocatorGet(_allocator));
        new(newNode) ANode(key);
        return newNode;
    }
    
    /// Free a node.
    void FreeNode(ANode* node)
    {
        (node)->~ANode();
        AllocatorFree(_allocator, node);
    }
    
    /// Rehash the buckets.
    void Rehash()
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            ANode* node = static_cast<ANode*>(it._ptr);
            unsigned hashKey = Hash(*it);
            node->_down = Ptrs()[hashKey];
            Ptrs()[hashKey] = node;
        }
    }
    
    /// Compare two nodes.
    static bool CompareNodes(ANode*& lhs, ANode*& rhs) { return lhs->key < rhs->key; }

    /// Compute a hash based on the _key and the bucket _size
    unsigned Hash(const _Ty& key) const { return MakeHash(key) & (NumBuckets() - 1); }
};

}
