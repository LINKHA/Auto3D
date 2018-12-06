#ifndef KH_STL_TYPE_MAP_TRAITS_H_
#define KH_STL_TYPE_MAP_TRAITS_H_
namespace KhSTL {

template<typename _Kty
	, typename _Ty>
	class tMapTraits
{
public:
	using KeyType = _Kty;
	using ValueType = tPair<_Kty, _Ty>;
};

}
#endif //!KH_STL_TYPE_MAP_TRAITS_H_