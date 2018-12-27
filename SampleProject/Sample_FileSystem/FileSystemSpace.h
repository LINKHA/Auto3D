#pragma once
#include "MotionSpace.h"


using namespace Auto3D;
class FreeCamera;
class FileSystemSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(FileSystemSpace)
public:
	void Init()override;
	void Destruct()override;
};


