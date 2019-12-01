#pragma once
#include "Source/Auto.h"

using namespace Auto3D;

class StartPage : public AObject
{
	REGISTER_OBJECT(StartPage, AObject)
public:
	static void DrawStartPage();
};