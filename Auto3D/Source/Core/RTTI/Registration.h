#include "AutoConfig.h"
#include "Container/String.h"
#include "Core/RTTI/Type.h"

namespace Auto3D
{
namespace RTTI
{

#define RTTR_CAT_IMPL(a, b) a##b
#define RTTR_CAT(a, b) RTTR_CAT_IMPL(a, b)

class AUTO_API FRegistration
{
	template<typename _ClassType>
	class Class
	{
	public:

		/// Construct a class_ object with the given name \p name.
		Class(FString name);

		~Class();
	};
};

template<typename _ClassType>
FRegistration::Class<_ClassType>::Class(FString name)
{
	auto t = FType::Get<_ClassType>();
	detail::type_register::custom_name(t, name);
	detail::type_register::register_visit_type_func(t, &detail::visit_type<Class_Type, Visitor_List>);
}

#define RTTR_REGISTRATION                                                           \
static void rttr_auto_register_reflection_function_();                              \
namespace                                                                           \
{                                                                                   \
struct rttr__auto__register__                                                   \
{                                                                               \
    rttr__auto__register__()                                                    \
    {                                                                           \
        rttr_auto_register_reflection_function_();                              \
    }                                                                           \
};                                                                              \
}                                                                                   \
static const rttr__auto__register__ RTTR_CAT(auto_register__, __LINE__);            \
static void rttr_auto_register_reflection_function_()

}
}