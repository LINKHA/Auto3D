#ifndef BASE_LIGHT_H_
#define BASE_LIGHT_H_
#include "Math/Color.h"
#include "GameObject.h"
USING_MATH
AUTO_BEGIN

class BaseLight : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(BaseLight, Component);
	DECLARE_OBJECT_SERIALIZE(BaseLight);
public:
	BaseLight();

private:
	Color m_Color;
};
AUTO_END
#endif //!BASE_LIGHT_H_
