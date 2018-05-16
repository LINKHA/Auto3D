#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_
#include "BaseLight.h"
AUTO_BEGIN
class DirectionalLight : public BaseLight
{
	REGISTER_DERIVED_CLASS(DirectionalLight, BaseLight);
	DECLARE_OBJECT_SERIALIZE(DirectionalLight);
public:
	DirectionalLight();

};
AUTO_END
#endif // !DIRECTIONAL_LIGHT_H_

