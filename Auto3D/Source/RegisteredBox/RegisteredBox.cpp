#include "RegisteredBox.h"
#include "../Debug/Log.h"

namespace Auto3D
{

RegisteredBox::RegisteredBox() :
	_activeScene(nullptr),
	_activeScene2d(nullptr),
	_activeCanvas(nullptr)
{
	RegisterSubsystem(this);
}
RegisteredBox::~RegisteredBox()
{
	RemoveSubsystem(this);
}

void RegisteredBox::SetActiveScene(Scene* scene)
{ 
	if (scene)
	{
		_activeScene = scene;
	}
	WarningString("File set active scene.");
}

void RegisteredBox::SetActiveScene2D(Scene2D* scene2d)
{
	if (scene2d)
	{
		_activeScene2d = scene2d;
	}
	WarningString("File set active scene.");
}

void RegisteredBox::SetActiveCanvas(Canvas* canva)
{
	if (canva)
	{
		_activeCanvas = canva;
	}
	WarningString("File set active scene.");
}

Scene* RegisteredBox::GetActiveScene()
{
	return _activeScene;
}

Scene2D* RegisteredBox::GetActiveScene2D()
{
	return _activeScene2d;
}
Canvas* RegisteredBox::GetActiveCanvas()
{
	return _activeCanvas;
}
}