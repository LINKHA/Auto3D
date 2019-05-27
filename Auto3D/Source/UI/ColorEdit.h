#pragma once
#include "UINode.h"

namespace Auto3D
{

class AUTO_API ColorEdit : public UINode
{
	REGISTER_OBJECT_CLASS(ColorEdit, UINode)
public:
	/// The constructor
	ColorEdit();
	/// The destructor
	~ColorEdit();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set text
	void SetText(const char* fmt);
	/// Set text
	void SetText(const String& fmt);
	/// Define node in ui
	void DefineNode()override;
private:
	/// Text string
	String _text;
	/// Color value
	Color _color;
};

}