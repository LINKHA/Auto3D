#include "Light.h"
#include "LogAssert.h"
AUTO_BEGIN

Light::Light(Ambient* ambi)
	:Super(ambi)
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