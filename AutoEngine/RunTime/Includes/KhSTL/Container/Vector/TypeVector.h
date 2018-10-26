#ifndef KH_STL_TYPE_VECTOR_H_
#define KH_STL_TYPE_VECTOR_H_


#include "TypeVectorAllocator.h"


#include <initializer_list>
#include <utility>
#include <cassert>
#include "../../Utility/TypeReverseIterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:6293)
#endif

namespace KhSTL {

/**
* Vector class
*/
template <typename _Ty,
	typename _Alloc = tAllocator<_Ty>>
	class tVector : public tVectorAlloc<_Ty, _Alloc>
{
public:
	using ValueType = _Ty;

	using This = tVector<_Ty, _Alloc>;

	using Base = tVectorAlloc<_Ty, _Alloc>;

	using Value = typename Base::Value;
	/// Allocator
	using Alloc = tVectorAlloc<_Ty, _Alloc>;
	/// Allocator type
	using AllocType = typename _Alloc::ValueType;

	using Iterator = tIterator<_Ty>;

	using ConstIterator = tConstIterator<_Ty>;
	/// List reverse iterator
	using ReverseIterator = tReverseIterator<Iterator>;
	/// List const reverse iterator
	using ConstReverseIterator = tReverseIterator<ConstIterator>;

	using CopyTag = typename Alloc::CopyTag;

	using MoveTag = typename Alloc::MoveTag;
public:
	/**
	* @brief : Construct Empty
	*/
	tVector() noexcept = default;
	/**
	* @brief : Construct with initial size
	*/
	explicit tVector(unsigned size)
	{
		Resize(size);
	}
	/**
	* @brief : Construct with initial size and default value
	*/
	tVector(unsigned size, const _Ty& value)
	{
		Resize(size);
		for (unsigned i = 0; i < size; ++i)
			At(i) = value;
	}
	/**
	* @brief : Construct with initial data
	*/
	tVector(const _Ty* data, unsigned size)
	{
		doInsertElements(0, data, data + size, CopyTag{});
	}
	/**
	* @brief : Copy-construct from another vector
	*/
	tVector(const tVector<_Ty>& vector)
	{
		doInsertElements(0, vector.Begin(), vector.End(), CopyTag{});
	}
	/**
	* @brief : Move-construct from another vector
	*/
	tVector(tVector<_Ty> && vector)
	{
		Value::Swap(vector);
	}
	/**
	* @brief : Aggregate initialization constructor
	*/
	tVector(const std::initializer_list<_Ty>& list)
		: tVector()
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			PushBack(*it);
		}
	}
	/**
	* @brief : Destruct
	*/
	~tVector()
	{
		Alloc::destructElements(Value::Buffer(), Value::_size);
		delete[] Value::_buffer;
	}

	/**
	* @brief : Assign from another vector
	*/
	tVector<_Ty>& operator =(const tVector<_Ty>& rhs)
	{
		// In case of This-assignment do nothing
		if (&rhs != this)
		{
			tVector<_Ty> copy(rhs);
			Swap(copy);
		}
		return *this;
	}
	/**
	* @brief : Move-assign from another vector
	*/
	tVector<_Ty>& operator =(tVector<_Ty> && rhs)
	{
		assert(&rhs != this);
		Swap(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign an element
	*/
	tVector<_Ty>& operator +=(const _Ty& rhs)
	{
		PushBack(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign another vector
	*/
	tVector<_Ty>& operator +=(const tVector<_Ty>& rhs)
	{
		PushBack(rhs);
		return *this;
	}
	/**
	* @brief : Add an element
	*/
	tVector<_Ty> operator +(const _Ty& rhs) const
	{
		tVector<_Ty> ret(*this);
		ret.PushBack(rhs);
		return ret;
	}
	/**
	* @brief : Add another vector
	*/
	tVector<_Ty> operator +(const tVector<_Ty>& rhs) const
	{
		tVector<_Ty> ret(*this);
		ret.PushBack(rhs);
		return ret;
	}
	/**
	* @brief : Test for equality with another vector
	*/
	bool operator ==(const tVector<_Ty>& rhs) const
	{
		if (rhs._size != Value::_size)
			return false;

		_Ty* buffer = Value::Buffer();
		_Ty* rhsBuffer = rhs.Buffer();
		for (unsigned i = 0; i < Value::_size; ++i)
		{
			if (buffer[i] != rhsBuffer[i])
				return false;
		}

		return true;
	}
	/**
	* @brief : Test for inequality with another vector
	*/
	bool operator !=(const tVector<_Ty>& rhs) const
	{
		if (rhs._size != Value::_size)
			return true;

		_Ty* buffer = Value::Buffer();
		_Ty* rhsBuffer = rhs.Buffer();
		for (unsigned i = 0; i < Value::_size; ++i)
		{
			if (buffer[i] != rhsBuffer[i])
				return true;
		}

		return false;
	}
	/**
	* @brief : Return element at index
	*/
	_Ty& operator [](unsigned index)
	{
		assert(index < Value::_size);
		return Value::Buffer()[index];
	}
	/**
	* @brief : Return const element at index
	*/
	const _Ty& operator [](unsigned index) const
	{
		assert(index < Value::_size);
		return Value::Buffer()[index];
	}
	/**
	* @brief : Return element at index
	*/
	_Ty& At(unsigned index)
	{
		assert(index < Value::_size);
		return Value::Buffer()[index];
	}
	/**
	* @brief : Return const element at index
	*/
	const _Ty& At(unsigned index) const
	{
		assert(index < Value::_size);
		return Value::Buffer()[index];
	}
	/**
	* @brief : Create an element at the end
	*/
	template <typename... Args>
	_Ty& EmplaceBack(Args&&... args)
	{
		if (Value::_size < Value::_capacity)
		{
			// Optimize common case
			++Value::_size;
			new (&Back()) _Ty(std::forward<Args>(args)...);
		}
		else
		{
			_Ty value(std::forward<Args>(args)...);
			PushBack(std::move(value));
		}
		return Back();
	}

#ifndef COVERITY_SCAN_MODEL
	/**
	* @brief : Add an element at the end
	*/
	void PushBack(const _Ty& value)
	{
		if (Value::_size < Value::_capacity)
		{
			// Optimize common case
			++Value::_size;
			new (&Back()) _Ty(value);
		}
		else
			doInsertElements(Value::_size, &value, &value + 1, CopyTag{});
	}
	/**
	* @brief : Add another vector at the end
	*/

	void PushBack(_Ty && value)
	{
		if (Value::_size < Value::_capacity)
		{
			// Optimize common case
			++Value::_size;
			new (&Back()) _Ty(std::move(value));
		}
		else
			doInsertElements(Value::_size, &value, &value + 1, MoveTag{});
	}
#else
	/**
	* @brief :FIXME: Attempt had been made to use this model in the Coverity-Scan model file without any success
	*			Probably because the model had generated a different mangled name than the one used by static analyzer
	*/
	void PushBack(const _Ty& value)
	{
		T array[] = { value };
		doInsertElements(_size, array, array + 1, CopyTag{});
	}
#endif
	/**
	* @brief : Add another vector at the end
	*/
	void PushBack(const tVector<_Ty>& vector)
	{
		doInsertElements(Value::_size, vector.Begin(), vector.End(), CopyTag{});
	}
	/**
	* @brief : Remove the last element
	*/
	void PopBack()
	{
		if (Value::_size)
			Resize(Value::_size - 1);
	}
	/**
	* @brief : Insert an element at position
	*/
	void Insert(unsigned pos, const _Ty& value)
	{
		doInsertElements(pos, &value, &value + 1, CopyTag{});
	}
	/**
	* @brief : Insert an element at position
	*/
	void Insert(unsigned pos, _Ty && value)
	{
		doInsertElements(pos, &value, &value + 1, MoveTag{});
	}
	/**
	* @brief : Insert another vector at position
	*/
	void Insert(unsigned pos, const tVector<_Ty>& vector)
	{
		doInsertElements(pos, vector.Begin(), vector.End(), CopyTag{});
	}
	/**
	* @brief : Insert an element by iterator
	*/
	Iterator Insert(const Iterator& dest, const _Ty& value)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, &value, &value + 1, CopyTag{});
	}
	/**
	* @brief : Move-insert an element by iterator
	*/
	Iterator Insert(const Iterator& dest, _Ty && value)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, &value, &value + 1, MoveTag{});
	}
	/**
	* @brief : Insert a vector by iterator
	*/
	Iterator Insert(const Iterator& dest, const tVector<_Ty>& vector)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, vector.Begin(), vector.End(), CopyTag{});
	}
	/**
	* @brief : Insert a vector partially by iterators
	*/
	Iterator Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, start, end, CopyTag{});
	}
	/**
	* @brief : Insert a vector partially by iterators
	*/
	Iterator Insert(const Iterator& dest, const Iterator& start, const Iterator& end)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, start, end, CopyTag{});
	}
	/**
	* @brief : Insert elements
	*/
	Iterator Insert(const Iterator& dest, const _Ty* start, const _Ty* end)
	{
		auto pos = (unsigned)(dest - Begin());
		return doInsertElements(pos, start, end, CopyTag{});
	}
	/**
	* @brief : Erase a range of elements
	*/
	void Erase(unsigned pos, unsigned length = 1)
	{
		// Return if the range is illegal
		if (pos + length > Value::_size || !length)
			return;

		doEraseElements(pos, length);
	}
	/**
	* @brief : Erase a range of elements by swapping elements from the end of the array
	*/
	void EraseSwap(unsigned pos, unsigned length = 1)
	{
		unsigned shiftStartIndex = pos + length;
		// Return if the range is illegal
		if (shiftStartIndex > Value::_size || !length)
			return;

		unsigned newSize = Value::_size - length;
		unsigned trailingCount = Value::_size - shiftStartIndex;
		if (trailingCount <= length)
		{
			// We're removing more elements from the array than exist past the end of the range being removed, so perform a normal shift and destroy
			doEraseElements(pos, length);
		}
		else
		{
			// Swap elements from the end of the array into the Empty space
			_Ty* buffer = Value::Buffer();
			std::move(buffer + newSize, buffer + Value::_size, buffer + pos);
			Resize(newSize);
		}
	}
	/**
	* @brief : Erase an element by iterator. Return iterator to the next element
	*/
	Iterator Erase(const Iterator& it)
	{
		auto pos = (unsigned)(it - Begin());
		if (pos >= Value::_size)
			return End();
		Erase(pos);

		return Begin() + pos;
	}
	/**
	* @brief : Erase a range by iterators. Return iterator to the next element
	*/
	Iterator Erase(const Iterator& start, const Iterator& end)
	{
		auto pos = (unsigned)(start - Begin());
		if (pos >= Value::_size)
			return End();
		auto length = (unsigned)(end - start);
		Erase(pos, length);

		return Begin() + pos;
	}
	/**
	* @brief : Erase an element by value.Return true if was found and erased
	*/
	bool Remove(const _Ty& value)
	{
		Iterator i = Find(value);
		if (i != End())
		{
			Erase(i);
			return true;
		}
		else
			return false;
	}
	/**
	* @brief : Erase an element by value by swapping with the last element.
				Return true if was found and erased
	*/
	bool RemoveSwap(const _Ty& value)
	{
		Iterator i = Find(value);
		if (i != End())
		{
			EraseSwap(i - Begin());
			return true;
		}
		else
			return false;
	}
	/**
	* @brief : Clear the vector
	*/
	void Clear() { Resize(0); }
	/**
	* @brief : Resize the vector
	*/
	void Resize(unsigned newSize) { doResize(newSize); }
	/**
	* @brief : Resize the vector and fill new elements with default value
	*/
	void Resize(unsigned newSize, const _Ty& value)
	{
		unsigned oldSize = Size();
		doResize(newSize);
		for (unsigned i = oldSize; i < newSize; ++i)
			At(i) = value;
	}
	/**
	* @brief : Set new capacity
	*/
	void Reserve(unsigned newCapacity)
	{
		if (newCapacity < Value::_size)
			newCapacity = Value::_size;

		if (newCapacity != Value::_capacity)
		{
			_Ty* newBuffer = nullptr;
			Value::_capacity = newCapacity;

			if (Value::_capacity)
			{
				newBuffer = reinterpret_cast<_Ty*>(Alloc::allocateBuffer((unsigned)(Value::_capacity * sizeof(_Ty))));
				// Move the data into the new buffer
				Alloc::constructElements(newBuffer, Begin(), End(), MoveTag{});
			}

			// Delete the old buffer
			Alloc::destructElements(Value::Buffer(), Value::_size);
			delete[] Value::_buffer;
			Value::_buffer = reinterpret_cast<unsigned char*>(newBuffer);
		}
	}
	/**
	* @brief : Reallocate so that no extra memory is used
	*/
	void Compact() { Reserve(Value::_size); }
	/**
	* @brief : Return iterator to value, or to the end if not found
	*/
	Iterator Find(const _Ty& value)
	{
		Iterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}
	/**
	* @brief : Return const iterator to value, or to the end if not found
	*/
	ConstIterator Find(const _Ty& value) const
	{
		ConstIterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}
	/**
	* @brief : Return index of value in vector, or size if not found
	*/
	unsigned IndexOf(const _Ty& value) const
	{
		return Find(value) - Begin();
	}
	/**
	* @brief : Return whether contains a specific value
	*/
	bool Contains(const _Ty& value) const { return Find(value) != End(); }
	/**
	* @brief : Return iterator to the beginning
	*/
	inline Iterator Begin() { return Iterator(Value::Buffer()); }
	/**
	* @brief : Return const iterator to the beginning
	*/
	inline ConstIterator Begin() const { return ConstIterator(Value::Buffer()); }

	/**
	* @brief : Return iterator to the end
	*/
	inline Iterator End() { return Iterator(Value::Buffer() + Value::_size); }
	/**
	* @brief : Return const iterator to the end
	*/
	inline ConstIterator End() const { return ConstIterator(Value::Buffer() + Value::_size); }

	inline ReverseIterator RBegin() { return ReverseIterator(End()); }

	inline ConstReverseIterator RBegin() const { return ReverseIterator(End()); }

	inline ReverseIterator REnd() { return ReverseIterator(Begin()); }

	inline ConstReverseIterator REnd() const { return ReverseIterator(Begin()); }

	/**
	* @brief : Return first element
	*/
	_Ty& Front()
	{
		assert(Value::_size);
		return Value::Buffer()[0];
	}
	/**
	* @brief : Return const first element
	*/
	const _Ty& Front() const
	{
		assert(Value::_size);
		return Value::Buffer()[0];
	}
	/**
	* @brief : Return last element
	*/
	_Ty& Back()
	{
		assert(Value::_size);
		return Value::Buffer()[Value::_size - 1];
	}
	/**
	* @brief : Return const last element
	*/
	const _Ty& Back() const
	{
		assert(Value::_size);
		return Value::Buffer()[Value::_size - 1];
	}
	/**
	* @brief : Return size of vector
	*/
	unsigned Size() const { return Value::_size; }
	/**
	* @brief : Return capacity of vector
	*/
	unsigned Capacity() const { return Value::_capacity; }
	/**
	* @brief : Return whether vector is Empty
	*/
	bool Empty() const { return Value::_size == 0; }

private:
	/**
	* @brief : Resize the vector and create/remove new elements as necessary
	*/
	void doResize(unsigned newSize)
	{
		// If size shrinks, destruct the removed elements
		if (newSize < Value::_size)
			destructElements(Value::Buffer() + newSize, Value::_size - newSize);
		else
		{
			// Allocate new buffer if necessary and copy the current elements
			if (newSize > Value::_capacity)
			{
				_Ty* src = Value::Buffer();

				// Reallocate vector
				tVector<_Ty> newVector;
				newVector.Reserve(calculateCapacity(newSize, Value::_capacity));
				newVector._size = Value::_size;
				_Ty* dest = newVector.Buffer();

				// Move old elements
				Alloc::constructElements(dest, src, src + Value::_size, MoveTag{});

				Swap(newVector);
			}

			// Initialize the new elements
			Alloc::constructElements(Value::Buffer() + Value::_size, newSize - Value::_size);
		}

		Value::_size = newSize;
	}
	/**
	* @brief : Insert elements into the vector using copy or move constructor
	*/
	template <typename _Tag, typename _RandomIterator>
	Iterator doInsertElements(unsigned pos, _RandomIterator start, _RandomIterator end, _Tag)
	{
		if (pos > Value::_size)
			pos = Value::_size;

		const unsigned numElements = end - start;
		if (Value::_size + numElements > Value::_capacity)
		{
			_Ty* src = Value::Buffer();

			// Reallocate vector
			tVector<_Ty> newVector;
			newVector.Reserve(Alloc::calculateCapacity(Value::_size + numElements, Value::_capacity));
			newVector._size = Value::_size + numElements;
			_Ty* dest = newVector.Buffer();

			// Copy or move new elements
			Alloc::constructElements(dest + pos, start, end, _Tag{});

			// Move old elements
			if (pos > 0)
				Alloc::constructElements(dest, src, src + pos, MoveTag{});
			if (pos < Value::_size)
				Alloc::constructElements(dest + pos + numElements, src + pos, src + Value::_size, MoveTag{});

			Value::Swap(newVector);
		}
		else if (numElements > 0)
		{
			_Ty* buffer = Value::Buffer();

			// Copy or move new elements
			Alloc::constructElements(buffer + Value::_size, start, end, _Tag{});

			// Rotate buffer
			if (pos < Value::_size)
			{
				std::rotate(buffer + pos, buffer + Value::_size, buffer + Value::_size + numElements);
			}

			// Update size
			Value::_size += numElements;
		}

		return Begin() + pos;
	}
	/**
	* @brief : Erase elements from the vector
	*/
	Iterator doEraseElements(unsigned pos, unsigned count)
	{
		assert(count > 0);
		assert(pos + count <= Value::_size);
		_Ty* buffer = Value::Buffer();
		std::move(buffer + pos + count, buffer + Value::_size, buffer + pos);
		Resize(Value::_size - count);
		return Begin() + pos;
	}
};


}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //!KH_STL_TYPE_VECTOR_H_