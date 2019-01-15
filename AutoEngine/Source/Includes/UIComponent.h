#pragma once
#include "Component.h"

namespace Auto3D {
/// %UI element horizontal alignment.
enum class AlignmentHorizontal
{
	Left = 0,
	Center,
	Right,
	Custom
};

/// %UI element vertical alignment.
enum class AlignmentVertical
{
	Top = 0,
	Center,
	Bottom,
	Custom
};

class UIComponent : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(UIComponent, Component)
public:
	explicit UIComponent(SharedPtr<Ambient> ambient);
	
};

}