#ifndef KH_STL_TYPE_DEFINE_H_
#define KH_STL_TYPE_DEFINE_H_

namespace KhSTL {

template<typename _Ty = void> struct tGreater
{	
	const bool operator()(const _Ty& lfs, const _Ty& rhs) const
	{	
		return lfs > rhs;
	}
};

template<typename _Ty = void> struct tGreaterEqual
{
	const bool operator()(const _Ty& lfs, const _Ty& rhs) const
	{	
		return lfs >= rhs;
	}
};

template<typename _Ty = void> struct tLess
{	
	const bool operator()(const _Ty& lfs, const _Ty& rhs) const
	{	
		return lfs < rhs;
	}
};

template<typename _Ty = void> struct tLessEqual
{
	const bool operator()(const _Ty& lfs, const _Ty& rhs) const
	{	
		return lfs <= rhs;
	}
};

template<typename _Ty> using Greater = KhSTL::tGreater<_Ty>;

template<typename _Ty> using GreaterEqual = KhSTL::tGreaterEqual<_Ty>;

template<typename _Ty> using Less = KhSTL::tLess<_Ty>;

template<typename _Ty> using LessEqual = KhSTL::tLessEqual<_Ty>;

}
#endif // KH_STL_TYPE_DEFINE_H_
