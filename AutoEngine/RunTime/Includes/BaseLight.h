#ifndef BASE_LIGHT_H_
#define BASE_LIGHT_H_
#include "BaseObject.h"
AUTO_BEGIN
class BaseLight : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(BaseLight, Object);
	DECLARE_OBJECT_SERIALIZE(BaseLight);
public:
	BaseLight();

};
AUTO_END
#endif //!BASE_LIGHT_H_
