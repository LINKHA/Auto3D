#pragma once
#include "UINode.h"

namespace Auto3D
{

class AUTO_API Slider : public UINode
{
	REGISTER_OBJECT_CLASS(Slider, UINode)
public:
	/// The constructor
	Slider();
	/// The destructor
	~Slider();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set text
	void SetText(const char* fmt);
	/// Set text
	void SetText(const String& fmt);
	/// Set Range min and max
	void SetRange(float minValue, float maxValue);
	/// Get slider value
	float GetValue() { return _value; }
	/// Define node in ui
	void DefineNode()override;
private:
	/// Text string
	String _text;
	/// Min value of slider
	float _minValue;
	/// Max value of slider
	float _maxValue;
	/// Value
	float _value;
};

}