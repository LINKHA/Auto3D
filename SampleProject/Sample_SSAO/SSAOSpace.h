#pragma  once
#include "Application.h"
using namespace Auto3D;
class SSAOSpace : public Application
{
	REGISTER_SPACE_CLASS(SSAOSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


