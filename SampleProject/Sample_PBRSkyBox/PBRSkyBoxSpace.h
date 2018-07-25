#pragma  once
#include "MotionSpace.h"
USING_AUTO
class PBRSkyBoxSpace : public MotionSpace
{
public:
	explicit PBRSkyBoxSpace(Ambient* ambient);
	~PBRSkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


