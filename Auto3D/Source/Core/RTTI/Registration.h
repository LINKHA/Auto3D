#include "AutoConfig.h"

namespace Auto3D
{

#define RTTR_CAT_IMPL(a, b) a##b
#define RTTR_CAT(a, b) RTTR_CAT_IMPL(a, b)

class AUTO_API FRegistration
{
public:
	template<typename...T>
	class bind;

	/*!
	 * The \ref class_ is used to register classes to RTTR.
	 */
	template<typename Class_Type, typename Visitor_List = READ_TL(rttr_visitor_list)>
	class Class
	{
	public:
		/*!
		 * \brief Construct a class_ object with the given name \p name.
		 *
		 * \param name The name of the class as string literal. Can be retrieved later via type::get_name().
		 *
		 */
		Class(string_view name);
		~Class();


		/*!
		 * \brief The bracket operator can be used to add additional meta data to the class type.
		 *
		 */
		template<typename...Args>
		Class<Class_Type, Visitor_List>& operator()(Args&&...args);


		/*!
		 * \brief Register a constructor for this class type with or without arguments.
		 *
		 * \param level  The access level of the constructor; default is: registration::public_access.<br>
		 *               (can be also: registration::protected_access or registration::private_access)
		 *
		 * \remark The default constructor create policy is: \ref policy::ctor::as_std_shared_ptr.
		 *
		 * \see constructor, type::get_constructor(), type::create()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename... Args, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_levels_list>::value>::type>
		bind<detail::ctor, Class_Type, acc_level, Visitor_List, Args...> Constructor(acc_level level = acc_level());

		/*!
		 * \brief Register a constructor for this class type which uses a function \p F.
		 *
		 * \param func   A function which creates an instance of \p Class_Type;
		 *               This can be a pointer to a function or a std::function.
		 * \param level  The access level of the constructor; default is: registration::public_access.<br>
		 *               (can be also: registration::protected_access or registration::private_access)
		 *
		 * \see constructor, type::get_constructor(), type::create()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename F, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<!detail::contains<F, detail::access_levels_list>::value>::type>
		bind<detail::ctor_func, Class_Type, F, acc_level, Visitor_List> Constructor(F func, acc_level level = acc_level());


		/*!
		 * \brief Register a property to this class.
		 *
		 * \param name  The name of the property.
		 * \param acc   The accessor to the property; this can be a pointer to a member or a pointer to a variable.
		 * \param level  The access level of the property; default is: registration::public_access.<br>
		 *               (can be also: registration::protected_access or registration::private_access)
		 *
		 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
		 *
		 * \see property, type::get_property(), type::get_property_value(), type::set_property_value()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename A, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_levels_list>::value>::type>
		bind<detail::prop, Class_Type, A, acc_level, Visitor_List> Property(string_view name, A acc, acc_level level = acc_level());

		/*!
		 * \brief Register a read only property to this class.
		 *
		 * \param name  The name of the property.
		 * \param acc   The accessor to the property; this can be a pointer to a member, a pointer to a variable,
		 *              a pointer to a member function, a pointer to a function or a std::function.
		 * \param level  The access level of the read only property; default is: registration::public_access.<br>
		 *               (can be: registration::protected_access or registration::private_access)
		 *
		 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
		 *
		 * \see property, type::get_property(), type::get_property_value(), type::set_property_value()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename A, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_levels_list>::value>::type>
		bind<detail::prop_readonly, Class_Type, A, acc_level, Visitor_List> property_readonly(string_view name, A acc, acc_level level = acc_level());

		/*!
		 * \brief Register a property to this class.
		 *
		 * \param name   The name of the property.
		 * \param getter The getter accessor to the property; this can be a pointer to a member function,
		 *               a pointer to a function or a std::function.
		 * \param setter The setter accessor to the property; this can be a pointer to a member function,
		 *               a pointer to a function or a std::function.
		 * \param level  The access level of the property; default is: registration::public_access.<br>
		 *               (can be also: registration::protected_access or registration::private_access)
		 *
		 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
		 *
		 * \see property, type::get_property(), type::get_property_value(), type::set_property_value()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename A1, typename A2, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<!detail::contains<A2, detail::access_levels_list>::value>::type>
		bind<detail::prop, Class_Type, A1, A2, acc_level, Visitor_List> Property(string_view name, A1 getter, A2 setter, acc_level level = acc_level());


		/*!
		 * \brief Register a method to this class.
		 *
		 * \param name      The name of the method.
		 * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
		 * \param level     The access level of the method; default is: registration::public_access.<br>
		 *                  (can be also: registration::protected_access or registration::private_access)
		 *
		 * \remark The method name does not have to be unique for this class.
		 *
		 * \see method, type::get_method(), type::invoke()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename F, typename acc_level = detail::public_access>
		bind<detail::meth, Class_Type, F, acc_level, Visitor_List> Method(string_view name, F f, acc_level level = acc_level());


		/*!
		 * \brief Register a nested enumeration of type \p Enum_Type
		 *
		 * \param name      The name of the enumeration.
		 *
		 * \see enumeration, type::get_enumeration()
		 *
		 * \return A \ref bind object, in order to chain more calls.
		 */
		template<typename Enum_Type>
		bind<detail::enum_, Class_Type, Enum_Type> enumeration(string_view name);
	private:
		Class(const std::shared_ptr<detail::registration_executer>& reg_exec);
		Class(const Class& other);
		Class& operator=(const Class& other);
	private:
		std::shared_ptr<detail::registration_executer> m_reg_exec;
		template<typename...T>
		friend class bind;

};

#define CLASS_REGISTRATION                                                          \
static void AutoRegisterReflectionFunction();										\
namespace                                                                           \
{                                                                                   \
    struct AutoRegister																\
    {                                                                               \
        AutoRegister()																\
        {                                                                           \
            AutoRegisterReflectionFunction();										\
        }                                                                           \
    };                                                                              \
}                                                                                   \
static const AutoRegister RTTR_CAT(autoRegister_,__LINE__);							\
static void RttrAutoRegisterReflectionFunction()

}