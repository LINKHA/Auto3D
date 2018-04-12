#ifndef STL_USE_H_
#define STL_USE_H_
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include "DefineLabels.h"



//#define STL_ALLOCATOR(label,t) AutoAllocator<t,label##ID>
#define MAKE_LABEL(label) label##ID

#define _LIST(t)					std::list<t>
#define _SET(t)						std::set<t,std::less<t>>
#define _VECTOR(t)					std::vector<t>
#define _PAIR(t,x)					std::pair(t,x)
#define M_PAIR(t,x)					std::make_pair(t,x)
#define AUTO_LIST(id,value)			std::list<std::pair<id,value>>
#define AUTO_SET(id,value)			std::set<pair<id,value>,std::less<pair<id,value>>>
#define AUTO_VECTOR(id,value)		std::vector<std::pair<id,value>>
#define AUTO_MAP(key, value)		std::map<key,value,std::less<key>>
#define AUTO_HASH_MAP(key, value)	std::unordered_map<key,value>
/**
*内存管理器(未完成暂时未进行使用)
*/
//template<typename T, LabelDefine label = STL_DO_LABEL(Default)>
//class AutoAllocator
//{
//public:
//	// typedefs...  
//	typedef T                   value_type;
//	typedef value_type*         pointer;
//	typedef value_type&         reference;
//	typedef value_type const*   const_pointer;
//	typedef value_type const&   const_reference;
//	typedef size_t              size_type;
//	typedef ptrdiff_t           difference_type;
//
//
//	// rebind...  
//	template <typename U> struct rebind { typedef AutoAllocator<U, label> other; };
//
//	AutoAllocator() {}
//
//	AutoAllocator(AutoAllocator<T, label> const &) {}
//
//	AutoAllocator<T, label> & operator=(AutoAllocator<T, label> const&) { return (*this); }
//
//
//
//	// function...
//	size_type max_size() const throw() { return 0x7fffffff; }
//
//	pointer address(reference x) const { return &x; }
//
//	void construct(pointer p, const_reference val) { :: new ((void *)p) T(val); }
//
//	void destroy(pointer p){ p->~T(); }
//
//
//
//	template <typename U, LabelDefine _memlabel>
//	bool operator== (AutoAllocator<U, _memlabel> const& a) const { return _memlabel == label;}
//	template <typename U, LabelDefine _memlabel>
//	bool operator!= (AutoAllocator<U, _memlabel> const& a) const { return _memlabel != label;}
//	pointer allocate(size_type n, const void *hint = 0)
//	{
//		T* tmp = static_cast<T*>(operator new[](n * sizeof((pointer)0)));
//		if (tmp == 0)
//			exit(1);
//		return tmp;
//	}
//	void deallocate(pointer p, size_type) { free(p); }
//
//};
//



#endif //! STL_USE_H_
