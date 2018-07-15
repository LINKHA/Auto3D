#pragma  once
#include "MotionSpace.h"
USING_AUTO
class SSAOSpace : public MotionSpace
{
public:
	SSAOSpace();
	~SSAOSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


