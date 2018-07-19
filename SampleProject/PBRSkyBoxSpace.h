#pragma  once
#include "MotionSpace.h"
USING_AUTO
class PBRSkyBoxSpace : public MotionSpace
{
public:
	PBRSkyBoxSpace();
	~PBRSkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


