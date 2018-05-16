#ifndef SPOT_LIGHT_H_
#define SPOT_LIGHT_H_
#include "BaseLight.h"

AUTO_BEGIN
class SpotLight : public BaseLight
{
	REGISTER_DERIVED_CLASS(SpotLight, BaseLight);
	DECLARE_OBJECT_SERIALIZE(SpotLight);
public:
	SpotLight();

};
AUTO_END
#endif // !SPOT_LIGHT_H_
