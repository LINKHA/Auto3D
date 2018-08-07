#include "LightSpot.h"

namespace Auto3D {


LightSpot::LightSpot(Ambient* ambi)
	:Super(ambi,kSpot)
{
	color.Set(1.0f, 1.0f, 1.0f);
	direction.Set(0.5f, -0.5f, -0.0f);
	cutOff = glm::cos(glm::radians(12.5f));
	outerCutOff = glm::cos(glm::radians(15.0f));
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	ambient.Set(0.3f, 0.3f, 0.3f);
	diffuse.Set(1.0f, 1.0f, 1.0f);
	specular.Set(0.7f, 0.7f, 0.7f);
}


LightSpot::~LightSpot()
{
}

}