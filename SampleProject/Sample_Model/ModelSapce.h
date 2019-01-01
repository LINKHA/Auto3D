#pragma  once
#include "Application.h"
using namespace Auto3D;
class ModelSapce : public Application
{
	REGISTER_SPACE_CLASS(ModelSapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


