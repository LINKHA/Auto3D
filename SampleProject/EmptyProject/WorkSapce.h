#pragma  once
#include "Application.h"

using namespace Auto3D;
class WorkSapce : public Application
{
	REGISTER_SPACE_CLASS(WorkSapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


