#include "BaseLight.h"

AUTO_BEGIN

BaseLight::BaseLight()
{
}
BaseLight::~BaseLight()
{
}
void BaseLight::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}
void BaseLight::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void BaseLight::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}
Color BaseLight::GetColor()
{
	return m_Color;
}

AUTO_END