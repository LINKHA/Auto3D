#pragma  once
#include "MotionSpace.h"
USING_AUTO
class SSAOSpace : public MotionSpace
{
public:
	explicit SSAOSpace(Ambient* ambient);
	~SSAOSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


