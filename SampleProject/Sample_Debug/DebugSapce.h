#pragma  once
#include "Application.h"

using namespace Auto3D;
class DebugSapce : public Application
{
	REGISTER_SPACE_CLASS(DebugSapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


