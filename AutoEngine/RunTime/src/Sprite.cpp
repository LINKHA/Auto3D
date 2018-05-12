#include "Sprite.h"


AUTO_BEGIN

void Node::addChild(const Sprite& node)
{
	m_Childs.push_back(node);
}
void Node::removeChild(arrayIndex index)
{
	if (m_Childs.begin() + index <= m_Childs.end())
		m_Childs.erase(m_Childs.begin() + index);
	else
		ErrorString("File remove child,the index is exceed child count\n");
}
Sprite Node::getChild(arrayIndex index)
{
	if (m_Childs.begin() + index >= m_Childs.end())
	{
		ErrorString("File get child,the index is exceed child count\n");
		return m_Childs.at(m_Childs.size());
	}
	return m_Childs.at(index);
}
Node::SpriteNodeArray Node::getAllChild()
{
	return m_Childs;
}


Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}




AUTO_END
