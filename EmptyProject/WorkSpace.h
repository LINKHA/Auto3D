#pragma  once
#include "MotionSpace.h"
USING_AUTO
class WorkSpace : public MotionSpace
{
public:
	WorkSpace();
	~WorkSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


