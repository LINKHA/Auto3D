/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_REGISTRATION_IMPL_H_
#define RTTR_REGISTRATION_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/argument_extractor.h"
#include "rttr/detail/constructor/constructor_wrapper.h"
#include "rttr/detail/constructor/constructor_wrapper_defaults.h"
#include "rttr/detail/destructor/destructor_wrapper.h"
#include "rttr/detail/enumeration/enumeration_wrapper.h"
#include "rttr/detail/method/method_wrapper.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/type/accessor_type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/type/type_register.h"
#include "rttr/detail/registration/bind_impl.h"
#include "rttr/detail/registration/registration_state_saver.h"
#include "rttr/policy.h"
#include "rttr/enumeration.h"
#include "rttr/detail/visitor/create_type_visitor_func.h"

#include <string>
#include <vector>

namespace Auto3D
{



/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE RTTI::metadata metadata(variant key, variant value)
{
    return RTTI::metadata{std::move(key), std::move(value)};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Enum_Type>
RTTR_INLINE RTTI::FEnumData<Enum_Type> value(string_view name, Enum_Type value)
{
    return RTTI::FEnumData<Enum_Type>(name, value);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename...TArgs>
RTTR_INLINE RTTI::default_args<TArgs...> default_arguments(TArgs&&...args)
{
    static_assert((sizeof...(TArgs) > 0), "Please provide at least one default argument!");
    return { std::forward<TArgs>(args)... };
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename...TArgs>
RTTR_INLINE RTTI::parameter_names<RTTI::decay_t<TArgs>...> parameter_names(TArgs&&...args)
{
    using namespace RTTI;
    static_assert(static_all_of<is_string_literal<raw_type_t<TArgs>>::value...>::value, "Please use this function only with string literals!");

    return { static_cast<RTTI::decay_t<TArgs>>(std::forward<TArgs>(args))...};
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
FRegistration::Class<Class_Type, Visitor_List>::Class(string_view name)
{
    auto t = type::get<Class_Type>();
    RTTI::type_register::custom_name(t, name);
    RTTI::type_register::register_visit_type_func(t, &RTTI::visit_type<Class_Type, Visitor_List>);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
FRegistration::Class<Class_Type, Visitor_List>::Class(const std::shared_ptr<RTTI::registration_executer>& reg_exec)
:   m_reg_exec(reg_exec)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
FRegistration::Class<Class_Type, Visitor_List>::~Class()
{
    // make sure that all base classes are registered
    RTTI::base_classes<Class_Type>::get_types();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename...Args>
FRegistration::Class<Class_Type, Visitor_List>& FRegistration::Class<Class_Type, Visitor_List>::operator()(Args&&...args)
{
    RTTI::type_register::metadata(type::get<Class_Type>(), RTTI::forward_to_vector<RTTI::metadata>(std::forward<Args>(args)...));
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename... Args, typename acc_level, typename Tp>
FRegistration::bind<RTTI::ctor, Class_Type, acc_level, Visitor_List, Args...> FRegistration::Class<Class_Type, Visitor_List>::Constructor(acc_level level)
{
    return {create_if_empty(m_reg_exec)};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename F, typename acc_level, typename Tp>
FRegistration::bind<RTTI::ctor_func, Class_Type, F, acc_level, Visitor_List> FRegistration::Class<Class_Type, Visitor_List>::Constructor(F func, acc_level level)
{
    using namespace RTTI;
    static_assert(is_functor<F>::value,
                  "No valid accessor for invoking the constructor provided!");

    static_assert(std::is_same<return_func, typename method_type<F>::type>::value,
                  "For creating this 'class type', please provide a function pointer or std::function with a return value.");

    return {create_if_empty(m_reg_exec), func};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename A, typename acc_level, typename Tp>
FRegistration::bind<RTTI::prop, Class_Type, A, acc_level, Visitor_List> FRegistration::Class<Class_Type, Visitor_List>::Property(string_view name, A acc, acc_level level)
{
    using namespace RTTI;
    static_assert(contains<acc_level, access_levels_list>::value, "The given type of 'level' is not a type of 'rttr::access_levels.'");
    static_assert(std::is_member_object_pointer<A>::value || std::is_pointer<A>::value,
                  "No valid property accessor provided!");

    return {create_if_empty(m_reg_exec), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename A, typename acc_level, typename Tp>
FRegistration::bind<RTTI::prop_readonly, Class_Type, A, acc_level, Visitor_List> FRegistration::Class<Class_Type, Visitor_List>::PropertyReadonly(string_view name, A acc, acc_level level)
{
    using namespace RTTI;
    static_assert(contains<acc_level, access_levels_list>::value, "The given type of 'level' is not a type of 'rttr::access_levels.'");
    static_assert(std::is_pointer<A>::value ||
                  std::is_member_object_pointer<A>::value || std::is_member_function_pointer<A>::value || is_functor<A>::value,
                  "No valid property accessor provided!");

    return {create_if_empty(m_reg_exec), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename A1, typename A2, typename acc_level, typename Tp>
FRegistration::bind<RTTI::prop, Class_Type, A1, A2, acc_level, Visitor_List> FRegistration::Class<Class_Type, Visitor_List>::Property(string_view name, A1 getter, A2 setter, acc_level level)
{
    using namespace RTTI;
    static_assert(contains<acc_level, access_levels_list>::value, "The given type of 'level' is not a type of 'rttr::access_levels.'");
    static_assert(std::is_member_function_pointer<A1>::value || std::is_member_function_pointer<A2>::value ||
                  is_functor<A1>::value || is_functor<A2>::value, "No valid property accessor provided!");

    static_assert(function_traits<A1>::arg_count == 0, "Invalid number of arguments, please provide as first accessor a getter-member-function without arguments.");
    static_assert(function_traits<A2>::arg_count == 1, "Invalid number of arguments, please provide as second argument a setter-member-function with exactly one argument.");
    using return_type   = typename function_traits<A1>::return_type;
    using arg_type      = typename param_types<A2, 0>::type;
    static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature (data type) for getter and setter!");

    return {create_if_empty(m_reg_exec), name, getter, setter};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename F, typename acc_level>
FRegistration::bind<RTTI::meth, Class_Type, F, acc_level, Visitor_List> FRegistration::Class<Class_Type, Visitor_List>::Method(string_view name, F f, acc_level level)
{
    using namespace RTTI;
    static_assert(contains<acc_level, access_levels_list>::value, "The given type of 'level' is not a type of 'rttr::access_levels.'");
    static_assert(std::is_member_function_pointer<F>::value || is_functor<F>::value, "No valid method accessor provided!");

    return {create_if_empty(m_reg_exec), name, f};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Visitor_List>
template<typename Enum_Type>
FRegistration::bind<RTTI::enum_, Class_Type, Enum_Type> FRegistration::Class<Class_Type, Visitor_List>::Enumeration(string_view name)
{
    using namespace RTTI;
    static_assert(std::is_enum<Enum_Type>::value, "No enum type provided, please call this method with an enum type!");

    return {create_if_empty(m_reg_exec), name};
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Visitor_List>
FRegistration::bind<RTTI::prop, RTTI::invalid_type, A, RTTI::public_access, Visitor_List> FRegistration::Property(string_view name, A acc)
{
    using namespace RTTI;
    static_assert(std::is_pointer<A>::value, "No valid property accessor provided!");
    return {std::make_shared<registration_executer>(), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Visitor_List>
FRegistration::bind<RTTI::prop_readonly, RTTI::invalid_type, A, RTTI::public_access, Visitor_List> FRegistration::property_readonly(string_view name, A acc)
{
    using namespace RTTI;
    static_assert(std::is_pointer<A>::value || is_functor<A>::value,
                 "No valid property accessor provided!");

    return {std::make_shared<registration_executer>(), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2, typename Visitor_List>
FRegistration::bind<RTTI::prop, RTTI::invalid_type, A1, A2, RTTI::public_access, Visitor_List> FRegistration::Property(string_view name, A1 getter, A2 setter)
{
    using namespace RTTI;
    static_assert(is_functor<A1>::value || is_functor<A2>::value,
                  "No valid property accessor provided!");

    return {std::make_shared<registration_executer>(), name, getter, setter};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Visitor_List>
FRegistration::bind<RTTI::meth, RTTI::invalid_type, F, RTTI::public_access, Visitor_List> FRegistration::Method(string_view name, F f)
{
    using namespace RTTI;
    static_assert(is_functor<F>::value, "No valid property accessor provided!");
    return {std::make_shared<registration_executer>(), name, f};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Enum_Type>
FRegistration::bind<RTTI::enum_, RTTI::invalid_type, Enum_Type> FRegistration::enumeration(string_view name)
{
    using namespace RTTI;
    static_assert(std::is_enum<Enum_Type>::value, "No enum type provided, please call this method with an enum type!");
    return {std::make_shared<registration_executer>(), name};
}

/////////////////////////////////////////////////////////////////////////////////////////

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


#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#define RTTR_PLUGIN_REGISTRATION RTTR_REGISTRATION
#else
#define RTTR_PLUGIN_REGISTRATION                                                    \
static void rttr_auto_register_reflection_function_() RTTR_DECLARE_PLUGIN_CTOR;     \
static void rttr_auto_unregister_reflection_function() RTTR_DECLARE_PLUGIN_DTOR;    \
                                                                                    \
static void rttr_auto_unregister_reflection_function()                              \
{                                                                                   \
    Auto3D::RTTI::get_registration_manager().unregister();                          \
}                                                                                   \
static void rttr_auto_register_reflection_function_()
#endif


#endif // RTTR_REGISTRATION_IMPL_H_
