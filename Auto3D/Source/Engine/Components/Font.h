#pragma once

#include "Container/Ptr.h"
#include "Resource/Resource.h"

namespace Auto3D
{

/// %AFont file type.
namespace EFontType
{
	enum Type
	{
		NONE = 0,
		FREE_TYPE,
		BIT_MAP,
		COUNT
	};
}


/// %AFont resource.
class AUTO_API AFont : public AResource
{
	DECLARE_CLASS(AFont, AResource)

public:
	/// Construct.
	AFont();
	/// Destruct.
	~AFont() override;

	/// Load resource from stream. May be called from a worker thread. Return true if successful.
	bool BeginLoad(FStream& source) override;
	/// Set absolute (in pixels) position adjustment for glyphs.
	void SetAbsoluteGlyphOffset(const TVector2I& offset);
	/// Set point size scaled position adjustment for glyphs.
	void SetScaledGlyphOffset(const TVector2F& offset);

	/// Return font type.
	EFontType::Type GetFontType() const { return _fontType; }

	/// Is signed distance field font.
	bool IsSDFFont() const { return _sdfFont; }

	/// Return absolute position adjustment for glyphs.
	const TVector2I& GetAbsoluteGlyphOffset() const { return _absoluteOffset; }

	/// Return point size scaled position adjustment for glyphs.
	const TVector2F& GetScaledGlyphOffset() const { return _scaledOffset; }

	/// Return the total effective offset for a point size.
	TVector2I GetTotalGlyphOffset(float pointSize) const;

	/// Return font data.
	unsigned char* Data() { return _fontData.Get(); }
	/// Return font data size.
	unsigned GetDataSize() { return _fontDataSize; }
private:
	/// AFont data.
	TSharedArrayPtr<unsigned char> _fontData;
	/// Size of font data.
	unsigned _fontDataSize;
	/// Absolute position adjustment for glyphs.
	TVector2I _absoluteOffset;
	/// Point size scaled position adjustment for glyphs.
	TVector2F _scaledOffset;
	/// AFont type.
	EFontType::Type _fontType;
	/// Signed distance field font flag.
	bool _sdfFont;
};

}
