#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class PBRSpace : public MotionSpace
{
public:
	explicit PBRSpace(Ambient* ambient);
	~PBRSpace();
	void Awake()override;
	int Launch();
};


