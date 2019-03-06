#include "Button.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Button::Button()
{

}

Button::~Button()
{

}

void Button::RegisterObject()
{
	RegisterFactory<Button>();
}

}