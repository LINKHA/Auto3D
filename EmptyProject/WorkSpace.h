#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class WorkSpace : public MotionSpace
{
public:
	explicit WorkSpace(Ambient* ambient);
	~WorkSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


