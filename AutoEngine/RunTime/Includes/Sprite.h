#ifndef SPRITE_H_
#define SPRITE_H_
#include "Component.h"
#include "stl_use.h"


AUTO_BEGIN
class Sprite;
class Node :public Object
{
public:
	Node()
	{}
	~Node()
	{}
	typedef int arrayIndex;
	typedef AUTO_HASH_MAP(ClassIDType, BaseComponent) ComponentsArray;
	typedef _VECTOR(Sprite) SpriteNodeArray;

	void enable();
	void destory();

	void SetLayer(int layerIndex);
	int GetLayer() const { return m_Layer; }

	void addComponent(const BaseComponent& Component);	//init addComponent
	void enableComponent();								//Make the component enable
	void dynAddComponent();								//Dynamic add component
	void getComponent();

	int GetComponentIndex(const BaseComponent& component);

	//int getComponentSize() { return m_Components.size(); }

	const Node& getSprite()const { return *this; }
	Node getSprite() { return *this; }
protected:
	ComponentsArray m_Components;
	SpriteNodeArray m_Childs;
	UInt32 m_Layer;
	UInt16 m_Tag;
	bool m_IsActive;

public:

	//Waiting test
	virtual void addChild(const Sprite& node);
	virtual void removeChild(arrayIndex index);
	virtual Sprite getChild(arrayIndex index);
	virtual SpriteNodeArray getAllChild();

};
class Sprite: public Node
{
public:
	Sprite();
	~Sprite();
private:
	
};


AUTO_END
#endif // SPRITE_H_