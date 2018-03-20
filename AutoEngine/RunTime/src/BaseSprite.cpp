#include "BaseSprite.h"


AUTO_BEGIN
class BaseComponent;


BaseSprite::BaseSprite()
{

}


BaseSprite::~BaseSprite()
{
}

void BaseSprite::addComponent(BaseComponent* Component)
{
	AssertIf(Component == NULL);
	{
		//m_Components.push_back(Component);
	}
	
}
AUTO_END
