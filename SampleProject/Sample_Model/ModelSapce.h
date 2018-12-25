#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class ModelSapce : public MotionSpace
{
public:
	explicit ModelSapce(Ambient* ambient);
	~ModelSapce();
	void Awake()override;
};


