#pragma once
#include "core_prerequisites.h"
#include "string_view.h"

#include <type_traits>

namespace rttr
{

class RTTR_API type
{
public:
	typedef uintptr_t type_id;

	/*!
		* \brief Returns a type object for the given template type \a T.
		*
		* \return type for the template type \a T.
		*/
	template<typename T>
	RTTR_LOCAL static type get() RTTR_NOEXCEPT;

}

}
#include "type_impl.h"