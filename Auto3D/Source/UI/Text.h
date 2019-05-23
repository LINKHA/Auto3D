#pragma once
#include "UINode.h"

namespace Auto3D
{

class AUTO_API Text : public UINode
{
	REGISTER_OBJECT_CLASS(Text, UINode)
public:
	/// The constructor
	Text();
	/// The destructor
	~Text();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set text
	void SetText(const char* fmt, ...);
	/// Set text
	void SetText(const String& fmt, ...);
	/// Set color
	void SetColor(const Color& color);
	/// Define node in ui
	void DefineNode()override;
private:
	/// Text string
	String _text;
	/// Text color
	Color _color;
};

}