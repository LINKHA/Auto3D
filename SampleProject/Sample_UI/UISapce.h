#pragma  once
#include "MotionSpace.h"
#include "GameObject.h"
using namespace Auto3D;
class UISapce : public MotionSpace
{
public:
	explicit UISapce(Ambient* ambient);
	~UISapce();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();

};


