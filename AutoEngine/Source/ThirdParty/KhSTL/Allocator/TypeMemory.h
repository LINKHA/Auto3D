#ifndef KH_STL_TYPE_MEMOEY_H_
#define KH_STL_TYPE_MEMOEY_H_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace KhSTL {
	/*
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		std::set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0)
		{
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	template<class T1, class  T2>
	inline void _construct(T1* p, const T2& value)
	{
		new(p)T1(value);
	}

	template<class T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}

	template <typename T>
	class tmAllocator
	{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		template<class U>
		struct rebind
		{
			typedef tmAllocator<U> other;
		};

		static pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		static void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}

		static void deallocate(void* p)
		{
			_deallocate(p);
		}

		void construct(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		pointer address(reference x)
		{
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}

		size_type max_size() const
		{
			return size_type(UINT_MAX / sizeof(T));
		}

	};

	
	template<class T, class _Alloc = tmAllocator<T>>
	class simple_alloc
	{
	public:
		static T* allocate(size_t n)
		{
			return 0 == n ? 0 : (T*)_Alloc::allocate(n * sizeof(T));
		}

		static T* allocate(void)
		{
			return (T*)_Alloc::allocate(sizeof(T));
		}

		static void deallocate(T* p, size_t n)
		{
			if (0 != n)_Alloc::deallocate(p, n * sizeof(T));
		}

		static void deallocate(void* p)
		{
			_Alloc::deallocate(p);
		}
	};
	*/

}

#endif //!KH_STL_TYPE_MEMOEY_H_