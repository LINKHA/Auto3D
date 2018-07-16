#pragma  once
#include "MotionSpace.h"
USING_AUTO
class PBRSpace : public MotionSpace
{
public:
	PBRSpace();
	~PBRSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


