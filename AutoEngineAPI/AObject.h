#pragma once
#include "Auto.h"

AUTO_API_BEGIN

class AObject
{
public:
	AObject();
	~AObject();
	static void Instantiate(AObject obj);
};
AUTO_API_END


