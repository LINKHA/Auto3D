#pragma  once
#include "Application.h"

using namespace Auto3D;

class Physics3DSpace : public Application
{
	REGISTER_SPACE_CLASS(Physics3DSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};


