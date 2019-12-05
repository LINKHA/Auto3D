#pragma once
namespace Auto3D
{
	/*!
 * This helper trait returns a vector with type object of all base classes.
 * When there is no type_list defined or the class has no base class, an empty vector is returned.
 */
template<typename T, typename Enable = void>
struct RTTR_LOCAL base_classes
{
	static RTTR_INLINE info_container get_types()
	{
		info_container result;
		return result;
	}
};

}