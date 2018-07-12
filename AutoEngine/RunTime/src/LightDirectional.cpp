#include "LightDirectional.h"


AUTO_BEGIN
LightDirectional::LightDirectional()
{
	m_Type = Directional;
	color.Set(1.0f, 1.0f, 1.0f);
	direction.Set(0.5f, -0.5f, -0.0f);
	ambient.Set(0.3f, 0.3f, 0.3f);
	diffuse.Set(1.0f, 1.0f, 1.0f);
	specular.Set(0.7f, 0.7f, 0.7f);
}


LightDirectional::~LightDirectional()
{
}
AUTO_END