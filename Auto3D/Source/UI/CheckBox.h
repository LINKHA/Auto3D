#pragma once
#include "UINode.h"

namespace Auto3D
{

class AUTO_API CheckBox : public UINode
{
	REGISTER_OBJECT_CLASS(CheckBox, UINode)
public:
	/// The constructor
	CheckBox();
	/// The destructor
	~CheckBox();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set text
	void SetText(const char* fmt);
	/// Set text
	void SetText(const String& fmt);
	/// Return value select state.
	const bool IsSelect() const { return _value; }
	/// Define node in ui
	void DefineNode()override;
private:
	/// Text string
	String _text;
	/// Check bool value
	bool _value;
};

}