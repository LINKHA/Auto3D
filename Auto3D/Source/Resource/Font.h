#pragma once

#include "../Container/Ptr.h"
#include "Resource.h"

namespace Auto3D
{
static const int FONT_TEXTURE_MIN_SIZE = 128;
static const int FONT_DPI = 96;

/// %Font file type.
enum FontType
{
	FONT_NONE = 0,
	FONT_FREETYPE,
	FONT_BITMAP,
	MAX_FONT_TYPES
};

/// %Font resource.
class AUTO_API Font : public Resource
{
	REGISTER_OBJECT_CLASS(Font, Resource)

public:
	/// Construct.
	Font();
	/// Destruct.
	~Font() override;
	/// Register object factory.
	static void RegisterObject();

	/// Load resource from stream. May be called from a worker thread. Return true if successful.
	bool BeginLoad(Stream& source) override;
	/// Set absolute (in pixels) position adjustment for glyphs.
	void SetAbsoluteGlyphOffset(const Vector2I& offset);
	/// Set point size scaled position adjustment for glyphs.
	void SetScaledGlyphOffset(const Vector2F& offset);

	/// Return font type.
	FontType GetFontType() const { return _fontType; }

	/// Is signed distance field font.
	bool IsSDFFont() const { return _sdfFont; }

	/// Return absolute position adjustment for glyphs.
	const Vector2I& GetAbsoluteGlyphOffset() const { return _absoluteOffset; }

	/// Return point size scaled position adjustment for glyphs.
	const Vector2F& GetScaledGlyphOffset() const { return _scaledOffset; }

	/// Return the total effective offset for a point size.
	Vector2I GetTotalGlyphOffset(float pointSize) const;

	/// Return font data.
	unsigned char* Data() { return _fontData.Get(); }
	/// Return font data size.
	unsigned unsigned GetDataSize() { return _fontDataSize; }
private:
	/// Font data.
	SharedArrayPtr<unsigned char> _fontData;
	/// Size of font data.
	unsigned _fontDataSize;
	/// Absolute position adjustment for glyphs.
	Vector2I _absoluteOffset;
	/// Point size scaled position adjustment for glyphs.
	Vector2F _scaledOffset;
	/// Font type.
	FontType _fontType;
	/// Signed distance field font flag.
	bool _sdfFont;
};

}
