#pragma  once
#include "Application.h"

using namespace Auto3D;
class Physics2DSapce : public Application
{
	REGISTER_SPACE_CLASS(Physics2DSapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
	
};


