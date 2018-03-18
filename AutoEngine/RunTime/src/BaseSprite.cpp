#include "BaseSprite.h"


AUTO_BEGIN
class BaseCompontent;


BaseSprite::BaseSprite()
{
	addComponent(Transform());
}


BaseSprite::~BaseSprite()
{
}

void BaseSprite::addComponent(const BaseCompontent& compontent)
{
	compontents.push_back(compontent);
}
AUTO_END
