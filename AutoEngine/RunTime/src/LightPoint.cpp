#include "LightPoint.h"


AUTO_BEGIN
LightPoint::LightPoint(Ambient* ambi)
	:Super(ambi)
{
	_type = kPoint;
	color.Set(1.0f, 1.0f, 1.0f);
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	ambient.Set(0.3f, 0.3f, 0.3f);
	diffuse.Set(1.0f, 1.0f, 1.0f);
	specular.Set(0.7f, 0.7f, 0.7f);
}


LightPoint::~LightPoint()
{
}
AUTO_END