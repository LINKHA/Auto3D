#ifndef BASESPRITE_H_
#define BASESPRITE_H_

#include "Auto.h"
#include "Transform.h"
#include "stl_use.h"



AUTO_BEGIN
class BaseSprite :public Object
{
public:
	BaseSprite();
	~BaseSprite(); 
	typedef TEMP_VECTOR(BaseComponent *) ComponentsArray;


	void enable();
	void destory();

	void SetLayer(int layerIndex);
	int GetLayer() const { return m_Layer; }

	void addComponent(BaseComponent* Component);	//init addComponent
	void enableComponent();								//Make the component enable
	void dynAddComponent();								//Dynamic add component
	void getComponent();

	int GetComponentIndex(BaseComponent *component);

	//int getComponentSize() { return m_Components.size(); }

	const BaseSprite& getSprite()const { return *this; }
	BaseSprite getSprite() { return *this; }
private:
	ComponentsArray m_Components;
	UInt32 m_Layer;
	UInt16 m_Tag;
	bool m_IsActive;

};
AUTO_END
#endif // BASESPRITE_H_