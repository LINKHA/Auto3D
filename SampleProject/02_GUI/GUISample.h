#pragma once
#include "../Sample.h"

using namespace Auto3D;

class GUISample : public Sample
{
	DECLARE_CLASS(GUISample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void UIDraw()override;

	void ShowExampleAppSimpleOverlay(bool* open);
	void ShowExampleAppLog(bool* open);
	void ShowExampleAppLayout(bool* open);

	ATexture* imgTexture;
};