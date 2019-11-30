#include "RegisteredBox.h"
#include "Debug/Log.h"

namespace Auto3D
{

ARegisteredBox::ARegisteredBox() :
	_activeScene(nullptr),
	_activeScene2d(nullptr)
{
}
ARegisteredBox::~ARegisteredBox()
{
}

void ARegisteredBox::SetActiveScene(AScene* scene)
{ 
	if (scene)
	{
		_activeScene = scene;
	}
	else
	{
		WarningString("File set active scene.");
	}
}

void ARegisteredBox::SetActiveScene2D(AScene2D* scene2d)
{
	if (scene2d)
	{
		_activeScene2d = scene2d;
	}
	else
	{
		WarningString("File set active scene 2D.");
	}
}

AScene* ARegisteredBox::GetActiveScene()
{
	return _activeScene;
}

AScene2D* ARegisteredBox::GetActiveScene2D()
{
	return _activeScene2d;
}

}