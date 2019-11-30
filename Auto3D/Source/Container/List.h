#pragma once

#include "Allocator.h"
#include "Swap.h"

namespace Auto3D
{

/// Doubly-linked list node base class.
struct AUTO_API FListNodeBase
{
    /// Construct.
    FListNodeBase() :
        _prev(nullptr),
        _next(nullptr)
    {
    }
    
    /// Previous node.
    FListNodeBase* _prev;
    /// Next node.
    FListNodeBase* _next;
};

/// Doubly-linked list iterator base class.
struct AUTO_API FListIteratorBase
{
    /// Construct.
    FListIteratorBase() :
        _ptr(nullptr)
    {
    }
    
    /// Construct with a node pointer.
    explicit FListIteratorBase(FListNodeBase* ptr) :
        _ptr(ptr)
    {
    }
    
    /// Test for equality with another iterator.
    bool operator == (const FListIteratorBase& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const FListIteratorBase& rhs) const { return _ptr != rhs._ptr; }
    
    /// Go to the next node.
    void GotoNext()
    {
        if (_ptr)
            _ptr = _ptr->_next;
    }
    
    /// Go to the previous node.
    void GotoPrev()
    {
        if (_ptr)
            _ptr = _ptr->_prev;
    }
    
    /// %ANode pointer.
    FListNodeBase* _ptr;
};

/// Doubly-linked list base class.
class AUTO_API FListBase
{
public:
    /// Construct.
    FListBase() :
        _ptrs(nullptr),
        _allocator(nullptr)
    {
    }
    
    /// Destruct.
    ~FListBase()
    {
        delete[] _ptrs;
    }

    /// Swap with another linked list.
    void Swap(FListBase& list)
    {
        Auto3D::Swap(_ptrs, list._ptrs);
        Auto3D::Swap(_allocator, list._allocator);
    }

    /// Return number of elements.
    size_t Size() const { return _ptrs ? (reinterpret_cast<size_t*>(_ptrs))[0] : 0; }
    /// Return whether has no elements.
    bool IsEmpty() const { return Size() == 0; }
    
protected:
    /// Allocate head & tail pointers + room for _size variable.
    void AllocatePtrs();
    /// Set new _size.
    void SetSize(size_t _size) { reinterpret_cast<size_t*>(_ptrs)[0] = _size; }
    /// Set new head node.
    void SetHead(FListNodeBase* head) { _ptrs[1] = head; }
    /// Set new tail node.
    void SetTail(FListNodeBase* tail) { _ptrs[2] = tail; }

    /// Return list head node.
    FListNodeBase* Head() const { return _ptrs ? _ptrs[1] : nullptr; }
    /// Return list tail node.
    FListNodeBase* Tail() const { return _ptrs ? _ptrs[2] : nullptr; }

    /// Head & tail pointers and list _size.
    FListNodeBase** _ptrs;
    /// %ANode allocator.
    FAllocatorBlock* _allocator;
};

/// Doubly-linked list template class. Elements can generally be assumed to be in non-continuous memory.
template <typename _Ty> class TList : public FListBase
{
public:
    /// %TList node.
    struct ANode : public FListNodeBase
    {
        /// Construct undefined.
        ANode()
        {
        }
        
        /// Construct with value.
        ANode(const _Ty& value) :
            _value(value)
        {
        }
        
        /// %ANode value.
        _Ty _value;
        
        /// Return next node.
        ANode* Next() const { return static_cast<ANode*>(_next); }
        /// Return previous node.
        ANode* Prev() { return static_cast<ANode*>(_prev); }
    };
    
    /// %TList iterator.
    struct Iterator : public FListIteratorBase
    {
        /// Construct.
        Iterator()
        {
        }
        
        /// Construct with a node pointer.
        explicit Iterator(ANode* ptr) :
            FListIteratorBase(ptr)
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
        
        /// Point to the node value.
        _Ty* operator -> () const { return &(static_cast<ANode*>(_ptr))->_value; }
        /// Dereference the node value.
        _Ty& operator * () const { return (static_cast<ANode*>(_ptr))->_value; }
    };
    
    /// %TList const iterator.
    struct ConstIterator : public FListIteratorBase
    {
        /// Construct.
        ConstIterator()
        {
        }
        
        /// Construct with a node pointer.
        explicit ConstIterator(ANode* ptr) :
            FListIteratorBase(ptr)
        {
        }
        
        /// Construct from a non-const iterator.
        ConstIterator(const Iterator& it) :
            FListIteratorBase(it.ptr)
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
        
        /// Point to the node value.
        const _Ty* operator -> () const { return &(static_cast<ANode*>(_ptr))->value; }
        /// Dereference the node value.
        const _Ty& operator * () const { return (static_cast<ANode*>(_ptr))->value; }
    };

    /// Construct empty.
    TList()
    {
    }
    
    /// Copy-construct.
    TList(const TList<_Ty>& list)
    {
        // Reserve the tail node + initial capacity according to the list's _size
        Initialize(list.Size() + 1);
        *this = list;
    }

    /// Destruct.
    ~TList()
    {
        if (_ptrs && _allocator)
        {
            Clear();
            FreeNode(Tail());
            AllocatorUninitialize(_allocator);
        }
    }
    
    /// Assign from another list.
    TList& operator = (const TList<_Ty>& rhs)
    {
        if (&rhs != this)
        {
            Clear();
            Insert(End(), rhs);
        }
        return *this;
    }
    
    /// Add-assign an element.
    TList& operator += (const _Ty& rhs)
    {
        Push(rhs);
        return *this;
    }
    
    /// Add-assign a list.
    TList& operator += (const TList<_Ty>& rhs)
    {
        Insert(End(), rhs);
        return *this;
    }
    
    /// Test for equality with another list.
    bool operator == (const TList<_Ty>& rhs) const
    {
        if (rhs.Size() != Size())
            return false;
        
        ConstIterator it = Begin();
        ConstIterator rhsIt = rhs.Begin();
        while (it != End())
        {
            if (*it != *rhsIt)
                return false;
            ++it;
            ++rhsIt;
        }
        
        return true;
    }
    
    /// Test for inequality with another list.
    bool operator != (const TList<_Ty>& rhs) const { return !(*this == rhs); }
    
    /// Insert an element to the end.
    void Push(const _Ty& value) { InsertNode(Tail(), value); }
    /// Insert an element to the beginning.
    void PushFront(const _Ty& value) { InsertNode(Head(), value); }
    /// Insert an element at _position.
    void Insert(const Iterator& dest, const _Ty& value) { InsertNode(static_cast<ANode*>(dest.ptr_), value); }
    
    /// Insert a list at _position.
    void Insert(const Iterator& dest, const TList<_Ty>& list)
    {
        ANode* destNode = static_cast<ANode*>(dest.ptr);
        for (ConstIterator it = list.Begin(); it != list.End(); ++it)
            destNode = InsertNode(destNode, *it)->Next();
    }
    
    /// Insert elements by iterators.
    void Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        ANode* destNode = static_cast<ANode*>(dest.ptr);
        ConstIterator it = start;
        while (it != end)
            destNode = InsertNode(destNode, *it++)->Next();
    }
    
    /// Insert elements.
    void Insert(const Iterator& dest, const _Ty* start, const _Ty* end)
    {
        ANode* destNode = static_cast<ANode*>(dest.ptr);
        const _Ty* ptr = start;
        while (ptr != end)
            destNode = InsertNode(destNode, *ptr++)->Next();
    }
    
    /// Erase the last element.
    void Pop()
    {
        if (Size())
            Erase(--End());
    }
    
    /// Erase the first element.
    void PopFront()
    {
        if (Size())
            Erase(Begin());
    }
    
    /// Erase an element by iterator. Return iterator to the next element.
    Iterator Erase(Iterator it)
    {
        return Iterator(EraseNode(static_cast<ANode*>(it._ptr)));
    }
    
    /// Erase a range by iterators. Return an iterator to the next element.
    Iterator Erase(const Iterator& start, const Iterator& end)
    {
        Iterator it = start;
        while (it != end)
            it = EraseNode(static_cast<ANode*>(it.ptr));
        
        return it;
    }
    
    /// Clear the list.
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
        }
    }
    
    /// Resize the list by removing or adding items at the end.
    void Resize(size_t newSize)
    {
        while (Size() > newSize)
            Pop();
        
        while (Size() < newSize)
            InsertNode(Tail(), _Ty());
    }
    
    /// Return iterator to value, or to the end if not found.
    Iterator Find(const _Ty& value)
    {
        Iterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }
    
    /// Return const iterator to value, or to the end if not found.
    ConstIterator Find(const _Ty& value) const
    {
        ConstIterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }
    
    /// Return whether contains a specific value.
    bool Contains(const _Ty& value) const { return Find(value) != End(); }
    /// Return iterator to the first element.
    Iterator Begin() { return Iterator(Head()); }
    /// Return const iterator to the first element.
    ConstIterator Begin() const { return ConstIterator(Head()); }
    /// Return iterator to the end.
    Iterator End() { return Iterator(Tail()); }
    /// Return const iterator to the end.
    ConstIterator End() const { return ConstIterator(Tail()); }
    /// Return first element.
    _Ty& Front() { return *Begin(); }
    /// Return const first element.
    const _Ty& Front() const { return *Begin(); }
    /// Return last element.
    _Ty& Back() { return *(--End()); }
    /// Return const last element.
    const _Ty& Back() const { return *(--End()); }
    
private:
    /// Return the head node.
    ANode* Head() const { return static_cast<ANode*>(FListBase::Head()); }
    /// Return the tail node.
    ANode* Tail() const { return static_cast<ANode*>(FListBase::Tail()); }
    
    /// Reserve the tail node and initial node capacity.
    void Initialize(size_t numNodes)
    {
        AllocatePtrs();
        _allocator = AllocatorInitialize(sizeof(ANode), numNodes);
        ANode* tail = AllocateNode();
        SetHead(tail);
        SetTail(tail);
    }

    /// Allocate and insert a node into the list. Return the new node.
    ANode* InsertNode(ANode* dest, const _Ty& value)
    {
        if (!dest)
        {
            // If not initialized yet, the only possibility is to insert before the tail
            if (!_ptrs)
            {
                Initialize(1);
                dest = Tail();
            }
            else
                return nullptr;
        }
        
        ANode* newNode = AllocateNode(value);
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
    
    /// Erase and free a node. Return pointer to the next node, or to the end if could not erase.
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
    
    /// Reserve a node with optionally specified value.
    ANode* AllocateNode(const _Ty& value = _Ty())
    {
        ANode* newNode = static_cast<ANode*>(AllocatorGet(_allocator));
        new(newNode) ANode(value);
        return newNode;
    }
    
    /// Free a node.
    void FreeNode(ANode* node)
    {
        (node)->~ANode();
        AllocatorFree(_allocator, node);
    }
};

}

