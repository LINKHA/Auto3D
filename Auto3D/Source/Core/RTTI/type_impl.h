#pragma once
#include "core_prerequisites.h"

#include <type_traits>

namespace rttr
{
template<typename T>
RTTR_INLINE type type::get() RTTR_NOEXCEPT
{
	using non_ref_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
	return detail::create_or_get_type<non_ref_type>();
}
}