#ifndef HDR_SPACE_H_
#define HDR_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class HDRSpace : public MotionSpace
{
public:
	HDRSpace();
	~HDRSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!HDR_SPACE_H_

