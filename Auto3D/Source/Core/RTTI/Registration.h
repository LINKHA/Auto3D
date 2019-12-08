#include "core_prerequisites.h"
#include "vistor_registration.h"
#include "string_view.h"
namespace rttr
{

class RTTR_API registration
{

template<typename Class_Type, typename Visitor_List = READ_TL(rttr_visitor_list)>
class class_
{
public:
	/*!
		* \brief Construct a class_ object with the given name \p name.
		*
		* \param name The name of the class as string literal. Can be retrieved later via type::get_name().
		*
		*/
	class_(string_view name);
	~class_();

};
#include "registration_imp.h"

}