#ifndef BASE_COMPONENT_H_
#define BASE_COMPONENT_H_

#include "Auto.h"
#include "BaseObject.h"



AUTO_BEGIN
class BaseComponent : public Object
{
public:
	BaseComponent();
	~BaseComponent();
	int getClassID();
};




AUTO_END

#endif // BASE_COMPONENT_H_
