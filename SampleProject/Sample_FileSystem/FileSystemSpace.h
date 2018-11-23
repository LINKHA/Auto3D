#pragma once
#include "MotionSpace.h"


using namespace Auto3D;
class FreeCamera;
class FileSystemSpace : public MotionSpace
{
public:
	explicit FileSystemSpace(Ambient* ambient);
	~FileSystemSpace();
	void Awake()override;
	int Launch();
};


