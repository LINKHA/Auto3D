#pragma once
#include "Light.h"

AUTO_BEGIN

class LightDirectional : public Light
{
	REGISTER_DERIVED_CLASS(LightDirectional, Light);
	DECLARE_OBJECT_SERIALIZE(LightDirectional);
public:
	LightDirectional();
public:
	/*Color color;
	Vector3 direction;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;*/
};
AUTO_END
