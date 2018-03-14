#include "BaseSprite.h"






AUTO_BEGIN
class CBaseCompontent;


BaseSprite::BaseSprite()
{
	addComponent(Transform());
}


BaseSprite::~BaseSprite()
{
}

void BaseSprite::addComponent(const CBaseCompontent& compontent)
{
	compontents.push_back(compontent);
}
AUTO_END
