#include "Water.h"

namespace Auto3D
{
REGISTER_CLASS(AWater)
{
	REGISTER_CALSS_FACTORY_IMP(AWater)
	.constructor<>()
	;
}

AWater::AWater()
{
}

AWater::~AWater()
{
}


}