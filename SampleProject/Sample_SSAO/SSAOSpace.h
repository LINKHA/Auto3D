#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class SSAOSpace : public MotionSpace
{
public:
	explicit SSAOSpace(Ambient* ambient);
	~SSAOSpace();
	void Awake()override;
};


