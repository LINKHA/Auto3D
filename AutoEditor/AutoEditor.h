#pragma once
#include "Source/Auto.h"

using namespace Auto3D;

class AutoEditor : public AApplication
{
	DECLARE_CLASS(AutoEditor, AApplication)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
private:
	void DrawMainPage();

	bool mainFlag = false;
};