#pragma once

#include "Allocator.h"
#include "Hash.h"
#include "Sort.h"
#include "Vector.h"

namespace Auto3D
{

/// Hash set template class.
template <typename _Ty> class HashSet : public HashBase
{
public:
    /// Hash set node.
    struct Node : public HashNodeBase
    {
        /// Construct undefined.
        Node()
        {
        }
        
        /// Construct with _key.
        Node(const _Ty& rhs) :
            _key(rhs)
        {
        }
        
        /// Key.
        _Ty _key;
        
        /// Return next node.
        Node* Next() const { return static_cast<Node*>(_next); }
        /// Return previous node.
        Node* Prev() const { return static_cast<Node*>(_prev); }
        /// Return next node in the bucket.
        Node* Down() const { return static_cast<Node*>(_down); }
    };
    
    /// Hash set node iterator.
    struct Iterator : public HashIteratorBase
    {
        /// Construct.
        Iterator()
        {
        }
        
        /// Construct with a node pointer.
        Iterator(Node* rhs) :
            HashIteratorBase(rhs)
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
        const _Ty* operator -> () const { return &(static_cast<Node*>(_ptr))->_key; }
        /// Dereference the _key.
        const _Ty& operator * () const { return (static_cast<Node*>(_ptr))->_key; }
    };
    
    /// Hash set node const iterator.
    struct ConstIterator : public HashIteratorBase
    {
        /// Construct.
        ConstIterator()
        {
        }
        
        /// Construct with a node pointer.
        ConstIterator(Node* rhs) :
            HashIteratorBase(rhs)
        {
        }
        
        /// Construct from a non-const iterator.
        ConstIterator(const Iterator& rhs) :
            HashIteratorBase(rhs.ptr)
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
        const _Ty* operator -> () const { return &(static_cast<Node*>(_ptr))->key; }
        /// Dereference the _key.
        const _Ty& operator * () const { return (static_cast<Node*>(_ptr))->key; }
    };
    
    /// Construct empty.
    HashSet()
    {
    }
    
    /// Construct from another hash set.
    HashSet(const HashSet<_Ty>& set)
    {
        Initialize(set.NumBuckets(), set.Size() + 1);
        *this = set;
    }
    
    /// Destruct.
    ~HashSet()
    {
        if (_ptrs && _allocator)
        {
            Clear();
            FreeNode(Tail());
            AllocatorUninitialize(_allocator);
        }
    }
    
    /// Assign a hash set.
    HashSet& operator = (const HashSet<_Ty>& rhs)
    {
        if (&rhs != this)
        {
            Clear();
            Insert(rhs);
        }
        return *this;
    }
    
    /// Add-assign a value.
    HashSet& operator += (const _Ty& rhs)
    {
        Insert(rhs);
        return *this;
    }
    
    /// Add-assign a hash set.
    HashSet& operator += (const HashSet<_Ty>& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /// Test for equality with another hash set.
    bool operator == (const HashSet<_Ty>& rhs) const
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
    bool operator != (const HashSet<_Ty>& rhs) const { return !(*this == rhs); }

    /// Insert a _key. Return an iterator to it.
    Iterator Insert(const _Ty& key)
    {
        unsigned hashKey = Hash(key);
        
        Node* existing = FindNode(key, hashKey);
        if (existing)
            return Iterator(existing);
        
        Node* newNode = InsertNode(Tail(), key);
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
    void Insert(const HashSet<_Ty>& set)
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
        
        Node* previous;
        Node* node = FindNode(key, hashKey, previous);
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
        
        Node* node = static_cast<Node*>(it.ptr);
        Node* next = node->Next();
        
        unsigned hashKey = Hash(node->key);
        
        Node* previous = nullptr;
        Node* current = static_cast<Node*>(Ptrs()[hashKey]);
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
                FreeNode(static_cast<Node*>(it++._ptr));
            
            Node* tail = Tail();
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
        
        Node** ptrs = new Node*[numKeys];
        Node* ptr = Head();
        
        for (size_t i = 0; i < numKeys; ++i)
        {
            ptrs[i] = ptr;
            ptr = ptr->Next();
        }
        
        Auto3D::Sort(RandomAccessIterator<Node*>(ptrs), RandomAccessIterator<Node*>(ptrs + numKeys), CompareNodes);
        
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
        Node* node = FindNode(key, hashKey);
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
        Node* node = FindNode(key, hashKey);
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
    Node* Head() const { return static_cast<Node*>(HashBase::Head()); }
    /// Return tail node with correct type.
    Node* Tail() const { return static_cast<Node*>(HashBase::Tail()); }

    /// Reserve the tail node and initial buckets.
    void Initialize(size_t numBuckets, size_t numNodes)
    {
        AllocateBuckets(0, numBuckets);
        _allocator = AllocatorInitialize(sizeof(Node), numNodes);
        HashNodeBase* tail = AllocateNode();
        SetHead(tail);
        SetTail(tail);
    }

    /// Find a node from the buckets.
    Node* FindNode(const _Ty& key, unsigned hashKey) const
    {
        if (!_ptrs)
            return nullptr;

        Node* node = static_cast<Node*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->_key == key)
                return node;
            node = node->Down();
        }
        
        return nullptr;
    }
    
    /// Find a node and the previous node from the buckets.
    Node* FindNode(const _Ty& key, unsigned hashKey, Node*& previous) const
    {
        previous = nullptr;
        if (!_ptrs)
            return nullptr;
        
        Node* node = static_cast<Node*>(Ptrs()[hashKey]);
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
    Node* InsertNode(Node* dest, const _Ty& key)
    {
        // If no pointers yet, allocate with minimum bucket count
        if (!_ptrs)
        {
            Initialize(MIN_BUCKETS, 2);
            dest = Head();
        }

        Node* newNode = AllocateNode(key);
        Node* prev = dest->Prev();
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
    Node* EraseNode(Node* node)
    {
        // The tail node can not be removed
        if (!node || node == Tail())
            return Tail();
        
        Node* prev = node->Prev();
        Node* next = node->Next();
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
    Node* AllocateNode(const _Ty& key = _Ty())
    {
        Node* newNode = static_cast<Node*>(AllocatorGet(_allocator));
        new(newNode) Node(key);
        return newNode;
    }
    
    /// Free a node.
    void FreeNode(Node* node)
    {
        (node)->~Node();
        AllocatorFree(_allocator, node);
    }
    
    /// Rehash the buckets.
    void Rehash()
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            Node* node = static_cast<Node*>(it._ptr);
            unsigned hashKey = Hash(*it);
            node->_down = Ptrs()[hashKey];
            Ptrs()[hashKey] = node;
        }
    }
    
    /// Compare two nodes.
    static bool CompareNodes(Node*& lhs, Node*& rhs) { return lhs->key < rhs->key; }

    /// Compute a hash based on the _key and the bucket _size
    unsigned Hash(const _Ty& key) const { return MakeHash(key) & (NumBuckets() - 1); }
};

}
