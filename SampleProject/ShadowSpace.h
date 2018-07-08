#ifndef SHADOW_SPACE_H_
#define SHADOW_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class ShadowSpace : public MotionSpace
{
public:
	ShadowSpace();
	~ShadowSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!WORK_SPACE_H_

