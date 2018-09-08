#pragma once
#include "Light.h"

namespace Auto3D {

class LightDirectional : public Light
{
	REGISTER_DERIVED_CLASS(LightDirectional, Light);
	DECLARE_OBJECT_SERIALIZE(LightDirectional);
public:
	explicit LightDirectional(Ambient* ambient);
public:
	/*Color color;
	Vector3 direction;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;*/
};
}
