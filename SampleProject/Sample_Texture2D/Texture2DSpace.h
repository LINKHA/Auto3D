#pragma  once
#include "Application.h"
using namespace Auto3D;
class Texture2DSpace : public Application
{
	REGISTER_SPACE_CLASS(Texture2DSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};


