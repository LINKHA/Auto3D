#ifndef KH_STL_TYPE_UNORDERED_MULTI_MAP_H_
#define KH_STL_TYPE_UNORDERED_MULTI_MAP_H_

namespace KhSTL {


template<typename _Kty,typename _Ty>
class tUnorderedMultiMap : public tHashBase
{
public:
	using KeyType = _Kty;

	using ValueType = _Ty;

	using PairType = tPair<_Kty, _Ty>;

	using Iterator = tHashIterator<PairType>;

	using ConstIterator = tConstHashIterator<PairType>;

	using Node = tHashNode<PairType>;
	/// Hash map reverse iterator
	using ReverseIterator = tReverseIterator<Iterator>;
	/// Hash map const reverse iterator
	using ConstReverseIterator = tReverseIterator<ConstIterator>;
public:
	/**
	* Construct Empty
	*/
	tUnorderedMultiMap()
	{
		// Reserve the tail node
		_allocator = AllocatorInitialize((unsigned)sizeof(Node));
		_head = _tail = ReserveNode();
	}
	/**
	* @brief : Construct from another hash map
	*/
	tUnorderedMultiMap(const tUnorderedMultiMap<_Kty, _Ty>& map)
	{
		// Reserve the tail node + initial capacity according to the map's size
		_allocator = AllocatorInitialize((unsigned)sizeof(Node), map.Size() + 1);
		_head = _tail = ReserveNode();
		*this = map;
	}
	/**
	* @brief : Move-construct from another hash map
	*/
	tUnorderedMultiMap(tUnorderedMultiMap<_Kty, _Ty> && map) noexcept
	{
		Swap(map);
	}
	/**
	* @brief : Aggregate initialization constructor
	*/
	tUnorderedMultiMap(const std::initializer_list<tPair<_Kty, _Ty>>& list) : tUnorderedMultiMap()
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			Insert(*it);
		}
	}
	/**
	* @brief : Destruct
	*/
	~tUnorderedMultiMap()
	{
		if (_allocator)
		{
			Clear();
			FreeNode(Tail());
			AllocatorUninitialize(_allocator);
			delete[] _ptrs;
		}
	}
	/**
	* @brief : Assign a hash map
	*/
	tUnorderedMultiMap& operator =(const tUnorderedMultiMap<_Kty, _Ty>& rhs)
	{
		// In case of This-assignment do nothing
		if (&rhs != this)
		{
			Clear();
			Insert(rhs);
		}
		return *this;
	}
	/**
	* @brief : Move-assign a hash map
	*/
	tUnorderedMultiMap& operator =(tUnorderedMultiMap<_Kty, _Ty> && rhs) noexcept
	{
		assert(&rhs != this);
		Swap(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign a pair
	*/
	tUnorderedMultiMap& operator +=(const tPair<_Kty, _Ty>& rhs)
	{
		Insert(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign a hash map
	*/
	tUnorderedMultiMap& operator +=(const tUnorderedMultiMap<_Kty, _Ty>& rhs)
	{
		Insert(rhs);
		return *this;
	}
	/**
	* @brief : _Keyest for equality with another hash map
	*/
	bool operator ==(const tUnorderedMultiMap<_Kty, _Ty>& rhs) const
	{
		if (rhs.Size() != Size())
			return false;

		ConstIterator i = Begin();
		while (i != End())
		{
			ConstIterator j = rhs.Find(i->first);
			if (j == rhs.End() || j->second != i->second)
				return false;
			++i;
		}

		return true;
	}
	/**
	* @brief : _Keyest for inequality with another hash map计算机专业大学生 大神
	*/
	bool operator !=(const tUnorderedMultiMap<_Kty, _Ty>& rhs) const
	{
		if (rhs.Size() != Size())
			return true;

		ConstIterator i = Begin();
		while (i != End())
		{
			ConstIterator j = rhs.Find(i->first);
			if (j == rhs.End() || j->second != i->second)
				return true;
			++i;
		}

		return false;
	}
	/**
	* @brief : Index the map. Create a new pair if key not found
	*/
	_Ty& operator [](const _Kty& key)
	{
		if (!_ptrs)
			return InsertNode(key, _Ty(), false)->data.second;

		unsigned hashKey = Hash(key);

		Node* node = FindNode(key, hashKey);
		return node ? node->data.second : InsertNode(key, _Ty(), false)->data.second;
	}
	/**
	* @brief : Index the map. Return null if key is not found, does not create a new pair
	*/
	_Ty* operator [](const _Kty& key) const
	{
		if (!_ptrs)
			return 0;

		unsigned hashKey = Hash(key);

		Node* node = FindNode(key, hashKey);
		return node ? &node->data.second : 0;
	}
	/**
	* @brief : Populate the map using variadic template. _Keyhis handles the Base case
	*/
	tUnorderedMultiMap& Populate(const _Kty& key, const _Ty& value)
	{
		this->operator [](key) = value;
		return *this;
	};
	/**
	* @brief : Populate the map using variadic template
	*/
	template <typename... Args> tUnorderedMultiMap& Populate(const _Kty& key, const _Ty& value, const Args&... args)
	{
		this->operator [](key) = value;
		return Populate(args...);
	};
	/**
	* @brief : Insert a pair. Return an iterator to it
	*/
	Iterator Insert(const tPair<_Kty, _Ty>& pair)
	{
		return Iterator(InsertNode(pair.first, pair.second));
	}
	/**
	* @brief : Insert a pair. Return iterator and set exists flag according to
	*			whether the key already existed.
	*/
	Iterator Insert(const tPair<_Kty, _Ty>& pair, bool& exists)
	{
		unsigned oldSize = Size();
		Iterator ret(InsertNode(pair.first, pair.second));
		exists = (Size() == oldSize);
		return ret;
	}
	/**
	* @brief : Insert a map
	*/
	void Insert(const tUnorderedMultiMap<_Kty, _Ty>& map)
	{
		ConstIterator it = map.Begin();
		ConstIterator end = map.End();
		while (it != end)
		{
			InsertNode(it->first, it->second);
			++it;
		}
	}
	/**
	* @brief : Insert a pair by iterator. Return iterator to the value
	*/
	Iterator Insert(const ConstIterator& it)
	{
		return Iterator(InsertNode(it->first, it->second));
	}
	/**
	* @brief : Insert a range by iterators
	*/
	void Insert(const ConstIterator& start, const ConstIterator& end)
	{
		ConstIterator it = start;
		while (it != end)
			InsertNode(*it++);
	}
	/**
	* @brief : Erase a pair by key. Return true if was found
	*/
	bool Erase(const _Kty& key)
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
			ptrs()[hashKey] = node->down;

		EraseNode(node);
		return true;
	}
	/**
	* @brief : Erase a pair by iterator. Return iterator to the next pair
	*/
	Iterator Erase(const Iterator& it)
	{
		if (!_ptrs || !it.ptr)
			return End();

		auto* node = static_cast<Node*>(it.ptr);
		Node* next = node->Next();

		unsigned hashKey = Hash(node->data.first);

		Node* previous = 0;
		auto* current = static_cast<Node*>(ptrs()[hashKey]);
		while (current && current != node)
		{
			previous = current;
			current = current->Down();
		}

		assert(current == node);

		if (previous)
			previous->down = node->down;
		else
			ptrs()[hashKey] = node->down;

		EraseNode(node);
		return Iterator(next);
	}
	/**
	* @brief : Clear the map
	*/
	void Clear()
	{
		// Prevent Find() from returning anything while the map is being cleared
		resetPtrs();

		if (Size())
		{
			for (Iterator i = Begin(); i != End();)
			{
				FreeNode(static_cast<Node*>(i++.ptr));
				i.ptr->prev = 0;
			}

			_head = _tail;
			setSize(0);
		}
	}
	/**
	* @brief : Sort pairs. After sorting the map can be iterated in order
	*			until new elements are inserted
	*/
	void Sort()
	{
		unsigned numKeys = Size();
		if (!numKeys)
			return;

		auto** ptrs = new Node*[numKeys];
		Node* ptr = Head();

		for (unsigned i = 0; i < numKeys; ++i)
		{
			ptrs[i] = ptr;
			ptr = ptr->Next();
		}

		Sort(tIterator<Node*>(ptrs), tIterator<Node*>(ptrs + numKeys), CompareNodes);

		_head = ptrs[0];
		ptrs[0]->prev = 0;
		for (unsigned i = 1; i < numKeys; ++i)
		{
			ptrs[i - 1]->next = ptrs[i];
			ptrs[i]->prev = ptrs[i - 1];
		}
		ptrs[numKeys - 1]->next = _tail;
		_tail->prev = ptrs[numKeys - 1];

		delete[] ptrs;
	}
	/**
	* @brief : Rehash to a specific bucket count, which must be a power of two. Return true if successful
	*/
	bool Rehash(unsigned numBuckets)
	{
		if (numBuckets == NumBuckets())
			return true;
		if (!numBuckets || numBuckets < Size() / MAX_LOAD_FACTOR)
			return false;

		// Check for being power of two
		unsigned check = numBuckets;
		while (!(check & 1u))
			check >>= 1;
		if (check != 1)
			return false;

		allocateBuckets(Size(), numBuckets);
		Rehash();
		return true;
	}
	/**
	* @brief : Return iterator to the pair with key, or end iterator if not found
	*/
	Iterator Find(const _Kty& key)
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
	/**
	* @brief : Return const iterator to the pair with key, or end iterator if not found
	*/
	ConstIterator Find(const _Kty& key) const
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
	/**
	* @brief : Return whether contains a pair with key
	*/
	bool Contains(const _Kty& key) const
	{
		if (!_ptrs)
			return false;

		unsigned hashKey = Hash(key);
		return FindNode(key, hashKey) != 0;
	}
	/**
	* @brief : try to copy value to output. Return true if was found
	*/
	bool TryGetValue(const _Kty& key, _Ty& out) const
	{
		if (!_ptrs)
			return false;
		unsigned hashKey = Hash(key);
		Node* node = FindNode(key, hashKey);
		if (node)
		{
			out = node->data.second;
			return true;
		}
		else
			return false;
	}
	/**
	* @brief : Return all the keys
	*/
	tVector<_Kty> Keys() const
	{
		tVector<_Kty> result;
		result.Reserve(Size());
		for (ConstIterator i = Begin(); i != End(); ++i)
			result.Push(i->first);
		return result;
	}
	/**
	* @brief : Return all the values
	*/
	tVector<_Ty> Values() const
	{
		tVector<_Ty> result;
		result.Reserve(Size());
		for (ConstIterator i = Begin(); i != End(); ++i)
			result.Push(i->second);
		return result;
	}
	/**
	* @brief : Return iterator to the beginning
	*/
	Iterator Begin() { return Iterator(Head()); }
	/**
	* @brief : Return iterator to the beginning
	*/
	ConstIterator Begin() const { return ConstIterator(Head()); }
	/**
	* @brief : Return iterator to the end
	*/
	Iterator End() { return Iterator(Tail()); }
	/**
	* @brief : Return iterator to the end
	*/
	ConstIterator End() const { return ConstIterator(Tail()); }


	inline ReverseIterator RBegin() { return ReverseIterator(End()); }

	inline ConstReverseIterator RBegin() const { return ReverseIterator(End()); }

	inline ReverseIterator REnd() { return ReverseIterator(Begin()); }

	inline ConstReverseIterator REnd() const { return ReverseIterator(Begin()); }
	/**
	* @brief : Return first pair
	*/
	const PairType& Front() const { return *Begin(); }
	/**
	* @brief : Return last pair
	*/
	const PairType& Back() const { return *(--End()); }

private:
	/**
	* @brief : Return the head node
	*/
	Node* Head() const { return static_cast<Node*>(_head); }
	/**
	* @brief : Return the tail node
	*/
	Node* Tail() const { return static_cast<Node*>(_tail); }
	/**
	* @brief : Find a node from the buckets. Do not call if the buckets have not been allocated
	*/
	Node* FindNode(const _Kty& key, unsigned hashKey) const
	{
		auto* node = static_cast<Node*>(ptrs()[hashKey]);
		while (node)
		{
			if (node->data.first == key)
				return node;
			node = node->Down();
		}

		return 0;
	}
	/**
	* @brief : Find a node and the previous node from the buckets. Do not call if the buckets have not been allocated
	*/
	Node* FindNode(const _Kty& key, unsigned hashKey, Node*& previous) const
	{
		previous = 0;

		auto* node = static_cast<Node*>(ptrs()[hashKey]);
		while (node)
		{
			if (node->data.first == key)
				return node;
			previous = node;
			node = node->Down();
		}

		return 0;
	}
	/**
	* @brief : Insert a key and value and return either the new or existing node
	*/
	Node* InsertNode(const _Kty& key, const _Ty& value, bool findExisting = false)
	{
		// If no pointers yet, allocate with minimum bucket count
		if (!_ptrs)
		{
			allocateBuckets(Size(), MIN_BUCKETS);
			Rehash();
		}

		unsigned hashKey = Hash(key);

		if (findExisting)
		{
			// If exists, just change the value
			Node* existing = FindNode(key, hashKey);
			if (existing)
			{
				existing->data.second = value;
				return existing;
			}
		}

		Node* newNode = InsertNode(Tail(), key, value);
		newNode->down = ptrs()[hashKey];
		ptrs()[hashKey] = newNode;

		// Rehash if the maximum load factor has been exceeded
		if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
		{
			allocateBuckets(Size(), NumBuckets() << 1);
			Rehash();
		}

		return newNode;
	}
	/**
	* @brief : Insert a node into the list. Return the new node
	*/
	Node* InsertNode(Node* dest, const _Kty& key, const _Ty& value)
	{
		if (!dest)
			return 0;

		Node* newNode = ReserveNode(key, value);
		Node* prev = dest->Prev();
		newNode->next = dest;
		newNode->prev = prev;
		if (prev)
			prev->next = newNode;
		dest->prev = newNode;

		// Reassign the head node if necessary
		if (dest == Head())
			_head = newNode;

		setSize(Size() + 1);

		return newNode;
	}
	/**
	* @brief : Erase a node from the list. Return pointer to the next element,
	*			or to the end if could not erase
	*/
	Node* EraseNode(Node* node)
	{
		// _Keyhe tail node can not be removed
		if (!node || node == _tail)
			return Tail();

		Node* prev = node->Prev();
		Node* next = node->Next();
		if (prev)
			prev->next = next;
		next->prev = prev;

		// Reassign the head node if necessary
		if (node == Head())
			_head = next;

		FreeNode(node);
		setSize(Size() - 1);

		return next;
	}
	/**
	* @brief : Reserve a node
	*/
	Node* ReserveNode()
	{
		auto* newNode = static_cast<Node*>(AllocatorReserve(_allocator));
		new(newNode) Node();
		return newNode;
	}
	/**
	* @brief : Reserve a node with specified key and value
	*/
	Node* ReserveNode(const _Kty& key, const _Ty& value)
	{
		auto* newNode = static_cast<Node*>(AllocatorReserve(_allocator));
		new(newNode) Node(tPair<_Kty, _Ty>(key, value));
		return newNode;
	}
	/**
	* @brief : Free a nod
	*/
	void FreeNode(Node* node)
	{
		(node)->~Node();
		AllocatorFree(_allocator, node);
	}
	/**
	* @brief : Rehash the buckets
	*/
	void Rehash()
	{
		for (Iterator i = Begin(); i != End(); ++i)
		{
			auto* node = static_cast<Node*>(i.ptr);
			unsigned hashKey = Hash(i->first);
			node->down = ptrs()[hashKey];
			ptrs()[hashKey] = node;
		}
	}

	/**
	* @brief : Compare two nodes
	*/
	static bool CompareNodes(Node*& lhs, Node*& rhs) { return lhs->data.first < rhs->data.first; }
	/**
	* @brief : Compute a hash based on the key and the bucket size
	*/
	unsigned Hash(const _Kty& key) const { return MakeHash(key) & (NumBuckets() - 1); }
};
template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ConstIterator begin(const tUnorderedMultiMap<_Kty, _Ty>& v) { return v.Begin(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ConstIterator end(const tUnorderedMultiMap<_Kty, _Ty>& v) { return v.End(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::Iterator begin(tUnorderedMultiMap<_Kty, _Ty>& v) { return v.Begin(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::Iterator end(tUnorderedMultiMap<_Kty, _Ty>& v) { return v.End(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ConstReverseIterator rbegin(const tUnorderedMultiMap<_Kty, _Ty>& v) { return v.RBegin(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ConstReverseIterator rend(const tUnorderedMultiMap<_Kty, _Ty>& v) { return v.REnd(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ReverseIterator rbegin(tUnorderedMultiMap<_Kty, _Ty>& v) { return v.RBegin(); }

template <typename _Kty, typename _Ty> typename tUnorderedMultiMap<_Kty, _Ty>::ReverseIterator rend(tUnorderedMultiMap<_Kty, _Ty>& v) { return v.REnd(); }

}

#endif // !KH_STL_TYPE_UNORDERED_MULTI_MAP_H_