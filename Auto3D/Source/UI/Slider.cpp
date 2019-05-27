#include "Slider.h"
#include "UI.h"
#include "../Debug/Log.h"
#include <imgui.h>

#include "../Debug/Log.h"

namespace Auto3D
{

Slider::Slider() :
	_text(String::EMPTY),
	_minValue(0.0f),
	_maxValue(1.0f),
	_value(0.0f)
{
}

Slider::~Slider() = default;

void Slider::RegisterObject()
{
	RegisterFactory<Slider>();
}

void Slider::SetText(const char* fmt)
{
	_text = fmt;
}
void Slider::SetText(const String& fmt)
{
	_text = fmt;
}

void Slider::SetRange(float minValue, float maxValue)
{
	if (_maxValue < _minValue)
	{
		WarningString("The min value > max value.");
		return;
	}
		
	_minValue = minValue;
	_maxValue = maxValue;
}

void Slider::DefineNode()
{
	if (_sameLineEnable)
		ImGui::SameLine(_offsetFromStartX, _spacingW);
	ImGui::SliderFloat(_text.CString(), &_value, _minValue, _maxValue);
}

}