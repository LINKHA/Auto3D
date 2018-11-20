#pragma  once
#include "MotionSpace.h"

using namespace Auto3D;
class Physics3DSpace : public MotionSpace
{
public:
	explicit Physics3DSpace(Ambient* ambient);
	~Physics3DSpace();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();

};


