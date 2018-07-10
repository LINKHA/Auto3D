#pragma once
#include "Auto.h"
#include "Singleton.h"
AUTO_BEGIN
class ShadowTest : public Singleton<ShadowTest>
{
public:
	ShadowTest();
	~ShadowTest();

	void BindDepathMap();
	int shadowWidth;
	int shadowHeight;

	unsigned int depthMapFBO;
	unsigned int depthMap;

};
AUTO_END
