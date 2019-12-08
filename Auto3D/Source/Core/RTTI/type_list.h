#pragma once
#include "core_prerequisites.h"

namespace rttr
{
	/*!
	 * \brief Contains a list of template parameters.
	 *
	 */
	template<typename...T>
	struct type_list
	{
		static RTTR_CONSTEXPR_OR_CONST auto size = sizeof...(T); //!< The amount of template parameters
	};


} // end namespace rttr