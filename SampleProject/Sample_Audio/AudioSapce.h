#pragma  once
#include "Application.h"
using namespace Auto3D;
class AudioSapce : public Application
{
	REGISTER_SPACE_CLASS(AudioSapce)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};