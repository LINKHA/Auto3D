#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class PBRSkyBoxSpace : public MotionSpace
{
public:
	explicit PBRSkyBoxSpace(Ambient* ambient);
	~PBRSkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


