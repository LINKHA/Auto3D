#include "Button.h"
#include <imgui.h>

namespace Auto3D
{

Button::Button() :
	_text(String::EMPTY),
	_click(false)
{}

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
	_text = fmt;
}

bool Button::IsClick()
{
	return _click;
}

void Button::DefineNode()
{
	if (_sameLineEnable)
		ImGui::SameLine(_offsetFromStartX, _spacingW);
	_click = ImGui::Button(_text.CString());
}

}