#pragma  once
#include "MotionSpace.h"
USING_AUTO
class HDREquirectangularSpace : public MotionSpace
{
public:
	HDREquirectangularSpace();
	~HDREquirectangularSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


