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
	void BindPointDepathMap();
	int shadowWidth;
	int shadowHeight;

	unsigned int depthMapFBO;
	unsigned int depthMap;

	unsigned int depthPointMapFBO;
	unsigned int depthPointmap;
};
AUTO_END
