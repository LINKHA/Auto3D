#ifndef AMBIENT_LIGHTING_H_
#define AMBIENT_LIGHTING_H_
#include "BaseLight.h"
AUTO_BEGIN
class AmbientLighting : public BaseLight
{
	REGISTER_DERIVED_CLASS(AmbientLighting, BaseLight);
	DECLARE_OBJECT_SERIALIZE(AmbientLighting);
public:
	AmbientLighting();

};
AUTO_END
#endif // !AMBIENT_LIGHTING_H_

