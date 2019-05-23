#include "Button.h"
#include <imgui.h>

namespace Auto3D
{

Button::Button() :
	_text(String::EMPTY)
{

}

Button::~Button() = default;

void Button::RegisterObject()
{
	RegisterFactory<Button>();
}

void Button::SetText(const char* fmt)
{
	_text = fmt;
}

void Button::SetText(const String& fmt)
{
	_text += fmt;
}

void Button::DefineNode()
{
	ImGui::Button(_text.CString());
}

}