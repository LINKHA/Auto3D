#include "RegisteredBox.h"
#include "Debug/Log.h"

namespace Auto3D
{

ARegisteredBox::ARegisteredBox() :
	_activeScene(nullptr),
	_activeScene2d(nullptr),
	_activeCanvas(nullptr)
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

void ARegisteredBox::SetActiveScene2D(Scene2D* scene2d)
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

void ARegisteredBox::SetActiveCanvas(Canvas* canva)
{
	if (canva)
	{
		_activeCanvas = canva;
	}
	else
	{
		WarningString("File set active canvas.");
	}
}

AScene* ARegisteredBox::GetActiveScene()
{
	return _activeScene;
}

Scene2D* ARegisteredBox::GetActiveScene2D()
{
	return _activeScene2d;
}
Canvas* ARegisteredBox::GetActiveCanvas()
{
	return _activeCanvas;
}
}