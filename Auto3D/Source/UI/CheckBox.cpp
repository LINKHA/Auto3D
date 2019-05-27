#include "CheckBox.h"
#include "UI.h"
#include "../Debug/Log.h"
#include <imgui.h>

#include "../Debug/Log.h"

namespace Auto3D
{

CheckBox::CheckBox() :
	_text(String::EMPTY),
	_value(false)
{
}

CheckBox::~CheckBox() = default;

void CheckBox::RegisterObject()
{
	RegisterFactory<CheckBox>();
}

void CheckBox::SetText(const char* fmt)
{
	_text = fmt;
}
void CheckBox::SetText(const String& fmt)
{
	_text = fmt;
}

void CheckBox::DefineNode()
{
	if (_sameLineEnable)
		ImGui::SameLine(_offsetFromStartX, _spacingW);

	ImGui::Checkbox(_text.CString(), &_value);
}

}