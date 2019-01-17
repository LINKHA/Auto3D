#pragma once
#include "Application.h"


using namespace Auto3D;
class FreeCamera;
class FileSystemSpace : public Application
{
	REGISTER_SPACE_CLASS(FileSystemSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};


