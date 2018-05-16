#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "BaseLight.h"

AUTO_BEGIN
class PointLight : public BaseLight
{
	REGISTER_DERIVED_CLASS(PointLight, BaseLight);
	DECLARE_OBJECT_SERIALIZE(PointLight);
public:
	PointLight();

};

AUTO_END

#endif // !POINT_LIGHT_H_