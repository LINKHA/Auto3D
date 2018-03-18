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
	void addComponent(const BaseCompontent& compontent);

private:
	AUTO_LIST(BaseCompontent) compontents;
};
AUTO_END
#endif // BASESPRITE_H_