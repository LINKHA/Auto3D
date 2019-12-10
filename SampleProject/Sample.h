#pragma once

#include "Source/Auto.h"

using namespace Auto3D;

class Sample : public AApplication
{
	DECLARE_CLASS(Sample, AApplication)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void CreateLogo();
protected:
	ACamera2D* logoCamera;
	TSharedPtr<AScene2D> scene2d;
};

#include "Sample.inl"