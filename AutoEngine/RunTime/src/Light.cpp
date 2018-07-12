#include "Light.h"
#include "LogAssert.h"
AUTO_BEGIN

Light::Light()
{
	AddToManager();
}
Light::~Light()
{}
void Light::AddToManager()
{
	INSTANCE(LightManager).AddLight(this);
}
void Light::RemoveFromManager()
{
	INSTANCE(LightManager).RemoveLight(this);
}

AUTO_END