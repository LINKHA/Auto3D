#pragma  once
#include "MotionSpace.h"
#include "GameObject.h"
using namespace Auto3D;
class Physics2DSapce : public MotionSpace
{
public:
	explicit Physics2DSapce(Ambient* ambient);
	~Physics2DSapce();
	void Awake()override;
	void Update()override;
	int Launch();

};


