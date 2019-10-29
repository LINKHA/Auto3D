#pragma once
#include "../Sample.h"

using namespace Auto3D;

class GUISample : public Sample
{
	REGISTER_OBJECT_CLASS(GUISample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void UIDraw()override;

	void ShowExampleAppSimpleOverlay(bool* open);
	void ShowExampleAppLog(bool* open);
	void ShowExampleAppLayout(bool* open);
};