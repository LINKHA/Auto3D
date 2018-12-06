#ifndef KH_STL_TYPE_SET_TRAITS_H_
#define KH_STL_TYPE_SET_TRAITS_H_
namespace KhSTL {

template<typename _Kty>
	class tSetTraits
{
public:
	using KeyType = _Kty;
	using ValueType = _Kty;
};


}
#endif //!KH_STL_TYPE_SET_TRAITS_H_