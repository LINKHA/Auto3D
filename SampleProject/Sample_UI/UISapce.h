#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class UISapce : public MotionSpace
{
public:
	explicit UISapce(Ambient* ambient);
	~UISapce();
	void Awake()override;
};


