#pragma once
#include "Source/Application.h"

using namespace Auto3D;

class AutoEditor : public Application
{
	REGISTER_OBJECT_CLASS(AutoEditor, Application)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};