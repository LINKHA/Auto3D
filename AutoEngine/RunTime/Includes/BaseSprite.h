#ifndef BASESPRITE_H_
#define BASESPRITE_H_

#include <vector>

#include "Auto.h"

#include "Transform.h"




AUTO_BEGIN
class BaseSprite
{
public:
	BaseSprite();
	~BaseSprite();
	void addComponent(const CBaseCompontent& compontent);

private:
	std::vector <CBaseCompontent> compontents;
};
AUTO_END
#endif // BASESPRITE_H_