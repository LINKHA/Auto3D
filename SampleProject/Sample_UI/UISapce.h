#pragma  once
#include "Application.h"
using namespace Auto3D;
class UISapce : public Application
{
	REGISTER_SPACE_CLASS(UISapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


