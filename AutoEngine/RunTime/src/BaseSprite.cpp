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
		//m_Components.push_back(make_pair(Component->getClassID,Component));
		m_Components.push_back( M_PAIR(Component->GetClassID(), Component));
	}
	
}
AUTO_END
