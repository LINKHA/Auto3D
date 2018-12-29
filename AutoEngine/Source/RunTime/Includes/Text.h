#pragma once
#include "UIComponent.h"


namespace Auto3D {
	
class Font;

class Text : public UIComponent
{
	REGISTER_OBJECT_CLASS(Text, UIComponent)
public:
	explicit Text(SharedPtr<Ambient> ambient);
	/**
	* @brief : Set text. Text is assumed to be either ASCII or UTF8-encoded
	*/
	void SetText(const STRING& text);

	void SetFont();

	void SetSize();

	void SetColor(const Color& color);
	/**
	* @brief : Set horizontal and vertical alignment
	*/
	void SetAlignment(AlignmentHorizontal hAlign, AlignmentVertical vAlign);
	/**
	* @brief : Set horizontal alignment
	*/
	void SetAlignmentHorizontal(AlignmentHorizontal align);
	/**
	* @brief : Set vertical alignment
	*/
	void SetAlignmentVertical(AlignmentVertical align);
};

}