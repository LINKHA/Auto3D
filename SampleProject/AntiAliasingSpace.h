#ifndef ANTI_AliASING_SPACE_H_
#define ANTI_AliASING_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class AntiAliasingSpace : public MotionSpace
{
public:
	AntiAliasingSpace();
	~AntiAliasingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!ANTI_AliASING_SPACE_H_

