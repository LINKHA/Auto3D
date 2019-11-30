#pragma once

#include "Allocator.h"
#include "Pair.h"
#include "Sort.h"
#include "Vector.h"

namespace Auto3D
{

/// Hash map template class.
template <typename _Ty1, typename _Ty2> class THashMap : public FHashBase
{
public:
    /// Hash map _key-value pair with const _key.
    class KeyValue
    {
    public:
        /// Default-construct.
        KeyValue() :
            _first(_Ty1())
        {
        }
        
        /// Construct with _key and value.
        KeyValue(const _Ty1& key, const _Ty2& value) :
            _first(key),
            _second(value)
        {
        }
		/// Prevent copy construction.
		KeyValue(const KeyValue& rhs) = delete;
		/// Prevent assignment.
		KeyValue& operator = (const KeyValue& rhs) = delete;
        /// Test for equality with another pair.
        bool operator == (const KeyValue& rhs) const { return _first == rhs._first && _second == rhs._second; }
        /// Test for inequality with another pair.
        bool operator != (const KeyValue& rhs) const { return !(*this == rhs); }
        
        /// Key.
        const _Ty1 _first;
        /// Value.
        _Ty2 _second;
    };
    
    /// Hash map node.
    struct ANode : public FHashNodeBase
    {
        /// Construct undefined.
        ANode()
        {
        }
        
        /// Construct with _key and value.
        ANode(const _Ty1& key, const _Ty2& value) :
            pair(key, value)
        {
        }
        
        /// Key-value pair.
        KeyValue pair;
        
        /// Return next node.
        ANode* Next() const { return static_cast<ANode*>(_next); }
        /// Return previous node.
        ANode* Prev() const { return static_cast<ANode*>(_prev); }
        /// Return next node in the bucket.
        ANode* Down() const { return static_cast<ANode*>(_down); }
    };
    
    /// Hash map node iterator.
    struct Iterator : public FHashIteratorBase
    {
        /// Construct.
        Iterator()
        {
        }
        
        /// Construct with a node pointer.
        Iterator(ANode* ptr) :
            FHashIteratorBase(ptr)
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
        
        /// Point to the pair.
        KeyValue* operator -> () const { return &(static_cast<ANode*>(_ptr))->pair; }
        /// Dereference the pair.
        KeyValue& operator * () const { return (static_cast<ANode*>(_ptr))->pair; }
    };
    
    /// Hash map node const iterator.
    struct ConstIterator : public FHashIteratorBase
    {
        /// Construct.
        ConstIterator()
        {
        }
        
        /// Construct with a node pointer.
        ConstIterator(ANode* ptr) :
            FHashIteratorBase(ptr)
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
        
        /// Point to the pair.
        const KeyValue* operator -> () const { return &(static_cast<ANode*>(_ptr))->pair; }
        /// Dereference the pair.
        const KeyValue& operator * () const { return (static_cast<ANode*>(_ptr))->pair; }
    };
    
    /// Construct empty.
    THashMap()
    {
    }
    
    /// Construct from another hash map.
    THashMap(const THashMap<_Ty1, _Ty2>& map)
    {
        Initialize(map.NumBuckets(), map.Size() + 1);
        *this = map;
    }
    
    /// Destruct.
    ~THashMap()
    {
        if (_ptrs && _allocator)
        {
            Clear();
            FreeNode(Tail());
            AllocatorUninitialize(_allocator);
        }
    }
    
    /// Assign a hash map.
    THashMap& operator = (const THashMap<_Ty1, _Ty2>& rhs)
    {
        if (&rhs != this)
        {
            Clear();
            Insert(rhs);
        }
        return *this;
    }
    
    /// Add-assign a pair.
    THashMap& operator += (const TPair<_Ty1, _Ty2>& rhs)
    {
        Insert(rhs);
        return *this;
    }
    
    /// Add-assign a hash map.
    THashMap& operator += (const THashMap<_Ty1, _Ty2>& rhs)
    {
        Insert(rhs);
        return *this;
    }
    
    /// Test for equality with another hash map.
    bool operator == (const THashMap<_Ty1, _Ty2>& rhs) const
    {
        if (rhs.Size() != Size())
            return false;
        
        for (ConstIterator it = Begin(); it != End(); ++it)
        {
            ConstIterator rhsIt = rhs.Find(it->_first);
            if (rhsIt == rhs.End() || rhsIt->_second != it->_second)
                return false;
        }
        
        return true;
    }
    
    /// Test for inequality with another hash map.
	bool operator != (const THashMap<_Ty1, _Ty2>& rhs) const { return !(*this == rhs); }

    /// Index the map. Create a new pair if _key not found.
    _Ty2& operator [] (const _Ty1& key)
    {
        return InsertNode(key)->pair._second;
    }
    
    /// Insert a pair. Return an iterator to it.
    Iterator Insert(const TPair<_Ty1, _Ty2>& pair)
    {
        return Iterator(InsertNode(pair._first, pair._second));
    }
    
    /// Insert a map.
    void Insert(const THashMap<_Ty1, _Ty2>& map)
    {
        for (ConstIterator it = map.Begin(); it != map.End(); ++it)
            InsertNode(it->_first, it->_second);
    }
    
    /// Insert a pair by iterator. Return iterator to the value.
    Iterator Insert(const ConstIterator& it) { return Iterator(InsertNode(it->_first, it->_second)); }
    
    /// Insert a range by iterators.
    void Insert(const ConstIterator& start, const ConstIterator& end)
    {
        ConstIterator it = start;
        while (it != end)
            Insert(*it++);
    }
    
    /// Erase a pair by _key. Return true if was found.
    bool Erase(const _Ty1& key)
    {
        if (!_ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        
        ANode* previous;
        ANode* node = FindNode(key, hashKey, previous);
        if (!node)
            return false;
        
        if (previous)
            previous->_down = node->_down;
        else
            Ptrs()[hashKey] = node->_down;
        
        EraseNode(node);
        return true;
    }
    
    /// Erase a pair by iterator. Return iterator to the next pair.
    Iterator Erase(const Iterator& it)
    {
        if (!_ptrs || !it._ptr)
            return End();
        
        ANode* node = static_cast<ANode*>(it._ptr);
        ANode* next = node->Next();
        
        unsigned hashKey = Hash(node->pair._first);
        
        ANode* previous = nullptr;
        ANode* current = static_cast<ANode*>(Ptrs()[hashKey]);
        while (current && current != node)
        {
            previous = current;
            current = current->Down();
        }
        
        assert(current == node);
        
        if (previous)
            previous->_down = node->_down;
        else
            Ptrs()[hashKey] = node->_down;
        
        EraseNode(node);
        return Iterator(next);
    }
    
    /// Clear the map.
    void Clear()
    {
        if (Size())
        {
            for (Iterator it = Begin(); it != End();)
                FreeNode(static_cast<ANode*>(it++._ptr));

            ANode* tail = Tail();
            tail->_prev = nullptr;
            SetHead(tail);
            SetSize(0);
            ResetPtrs();
        }
    }
    
    /// Sort pairs. After sorting the map can be iterated in order until new elements are inserted.
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
    
    /// Return iterator to the pair with _key, or end iterator if not found.
    Iterator Find(const _Ty1& key)
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
    
    /// Return const iterator to the pair with _key, or end iterator if not found.
    ConstIterator Find(const _Ty1& key) const
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
    
    /// Return whether contains a pair with _key.
    bool Contains(const _Ty1& key) const
    {
        if (!_ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        return FindNode(key, hashKey) != nullptr;
    }
    
    /// Return all the keys.
    TVector<_Ty1> Keys() const
    {
        TVector<_Ty1> result;
        result.Reserve(Size());
        for (ConstIterator it = Begin(); it != End(); ++it)
            result.Push(it->first);
        return result;
    }

    /// Return all the values.
    TVector<_Ty2> Values() const
    {
        TVector<_Ty2> result;
        result.Reserve(Size());
        for (ConstIterator it = Begin(); it != End(); ++it)
            result.Push(it->second);
        return result;
    }

    /// Return iterator to the first element. Is not the lowest _key unless the map has been sorted.
    Iterator Begin() { return Iterator(Head()); }
    /// Return const iterator to the beginning.
    ConstIterator Begin() const { return ConstIterator(Head()); }
    /// Return iterator to the end.
    Iterator End() { return Iterator(Tail()); }
    /// Return const iterator to the end.
    ConstIterator End() const { return ConstIterator(Tail()); }
    /// Return first keyvalue. Is not the lowest _key unless the map has been sorted.
    const _Ty1& Front() const { return *Begin(); }
    /// Return last keyvalue.
    const _Ty1& Back() const { assert(Size()); return *(--End()); }
    
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
    ANode* FindNode(const _Ty1& key, unsigned hashKey) const
    {
        if (!_ptrs)
            return nullptr;

        ANode* node = static_cast<ANode*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->pair._first == key)
                return node;
            node = node->Down();
        }
        
        return nullptr;
    }
    
    /// Find a node and the previous node from the buckets.
    ANode* FindNode(const _Ty1& key, unsigned hashKey, ANode*& previous) const
    {
        previous = nullptr;
        if (!_ptrs)
            return nullptr;

        ANode* node = static_cast<ANode*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->pair._first == key)
                return node;
            previous = node;
            node = node->Down();
        }
        
        return nullptr;
    }
    
    /// Insert a _key and default value and return either the new or existing node.
    ANode* InsertNode(const _Ty1& key)
    {
        unsigned hashKey = Hash(key);

        // If exists, just return the node
        ANode* existing = FindNode(key, hashKey);
        if (existing)
            return existing;

        ANode* newNode = InsertNode(Tail(), key, _Ty2());
        newNode->_down = Ptrs()[hashKey];
        Ptrs()[hashKey] = newNode;

        // Rehash if the maximum load factor has been exceeded
        if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
        {
            AllocateBuckets(Size(), NumBuckets() << 1);
            Rehash();
        }

        return newNode;
    }

    /// Insert a _key and value and return either the new or existing node.
    ANode* InsertNode(const _Ty1& key, const _Ty2& value)
    {
        unsigned hashKey = Hash(key);
        
        // If exists, just change the value
        ANode* existing = FindNode(key, hashKey);
        if (existing)
        {
            existing->pair._second = value;
            return existing;
        }
        
        ANode* newNode = InsertNode(Tail(), key, value);
        newNode->_down = Ptrs()[hashKey];
        Ptrs()[hashKey] = newNode;
        
        // Rehash if the maximum load factor has been exceeded
        if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
        {
            AllocateBuckets(Size(), NumBuckets() << 1);
            Rehash();
        }
        
        return newNode;
    }
    
    /// Allocate and insert a node into the list. Return the new node.
    ANode* InsertNode(ANode* dest, const _Ty1& key, const _Ty2& value)
    {
        // If no pointers yet, allocate with minimum bucket count
        if (!_ptrs)
        {
            Initialize(MIN_BUCKETS, 2);
            dest = Head();
        }
        
        ANode* newNode = AllocateNode(key, value);
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
            prev->_next = next;
        next->_prev = prev;
        
        // Reassign the head node if necessary
        if (node == Head())
            SetHead(next);
        
        FreeNode(node);
        SetSize(Size() - 1);
        
        return next;
    }
    
    /// Allocate a node with optionally specified _key and value.
    ANode* AllocateNode(const _Ty1& key = _Ty1(), const _Ty2& value = _Ty2())
    {
        ANode* newNode = static_cast<ANode*>(AllocatorGet(_allocator));
        new(newNode) ANode(key, value);
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
            unsigned hashKey = Hash(it->_first);
            node->_down = Ptrs()[hashKey];
            Ptrs()[hashKey] = node;
        }
    }
    
    /// Compare two nodes.
    static bool CompareNodes(ANode*& lhs, ANode*& rhs) { return lhs->pair.first < rhs->pair.first; }
    
    /// Compute a hash based on the _key and the bucket _size
    unsigned Hash(const _Ty1& key) const { return MakeHash(key) & (NumBuckets() - 1); }
};

}
