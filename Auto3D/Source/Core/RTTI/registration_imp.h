#pragma once
namespace rttr
{
template<typename Class_Type, typename Visitor_List>
registration::class_<Class_Type, Visitor_List>::class_(string_view name)
{
	auto t = type::get<Class_Type>();
	detail::type_register::custom_name(t, name);
	detail::type_register::register_visit_type_func(t, &detail::visit_type<Class_Type, Visitor_List>);
}
}
