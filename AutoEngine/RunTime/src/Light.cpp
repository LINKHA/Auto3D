#include "Light.h"
#include "LogAssert.h"

AUTO_BEGIN

Light::Light()
{}
Light::Light(LightType type)
	: m_Type(type)
{
	ambient.Set(0.3f, 0.3f, 0.3f);
	diffuse.Set(1.0f, 1.0f, 1.0f);
	specular.Set(0.7f, 0.7f, 0.7f);
	direction.Set(0.5f, -0.5f, -0.0f);
	color.Set(1.0f, 1.0f, 1.0f);
	cutOff = glm::cos(glm::radians(12.5f));
	outerCutOff = glm::cos(glm::radians(15.0f));
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

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

void Light::ComputeLight()
{
	/*if (m_Type == Directional)
	{
		Vector3 direction;
	}*/
	/*else if (m_Type == Point)
	{
		Vector3 position;
		float constant;
		float linear;
		float quadratic;
	}
	else if (m_Type == Spot)
	{
		m_position = GetGameObject().GetComponent(Transform).GetPosition();
		Vector3 direction;
		float cutOff;
		float outerCutOff;
		float constant;
		float linear;
		float quadratic;
	
	else
	{
		WarningString("Light fail to set data.");
	}}*/
}
AUTO_END