#pragma  once
#include "Application.h"
using namespace Auto3D;
class PBRSpace : public Application
{
	REGISTER_SPACE_CLASS(PBRSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};