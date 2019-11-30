#pragma once

#include "Source/Auto.h"

using namespace Auto3D;

class Sample : public AApplication
{
	REGISTER_OBJECT_CLASS(Sample, AApplication)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void CreateLogo();
protected:
	Camera2D* logoCamera;
	TSharedPtr<Scene2D> scene2d;
};

#include "Sample.inl"