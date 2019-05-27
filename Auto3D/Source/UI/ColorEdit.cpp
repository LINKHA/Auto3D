#include "ColorEdit.h"
#include "UI.h"
#include "../Debug/Log.h"
#include <imgui.h>

#include "../Debug/Log.h"

namespace Auto3D
{
static ImVec4 sColor;

ColorEdit::ColorEdit() :
	_text(String::EMPTY),
	_color(Color::WHITE)
{
}

ColorEdit::~ColorEdit() = default;

void ColorEdit::RegisterObject()
{
	RegisterFactory<ColorEdit>();
}

void ColorEdit::SetText(const char* fmt)
{
	_text = fmt;
}
void ColorEdit::SetText(const String& fmt)
{
	_text = fmt;
}

void ColorEdit::DefineNode()
{
	if (_sameLineEnable)
		ImGui::SameLine(_offsetFromStartX, _spacingW);

	sColor.x = _color._r;
	sColor.y = _color._g;
	sColor.z = _color._b;
	sColor.w = _color._a;

	ImGui::ColorEdit3(_text.CString(), (float*)&sColor);
	_color._r = sColor.x;
	_color._g = sColor.y;
	_color._b = sColor.z;
	_color._a = sColor.w;
}

}