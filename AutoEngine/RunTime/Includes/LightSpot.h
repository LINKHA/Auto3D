#pragma once
#include "Light.h"

namespace Auto3D {

class LightSpot : public Light
{
	REGISTER_DERIVED_CLASS(LightSpot, Light);
	DECLARE_OBJECT_SERIALIZE(LightSpot);
public:
	explicit LightSpot(Ambient* ambient);

public:
	/*Color color;
	Vector3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;*/

};

}
