#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class DebugSapce : public MotionSpace
{
public:
	explicit DebugSapce(Ambient* ambient);
	~DebugSapce();
	void Start()override;
	void Update()override;
	int Launch();
};

