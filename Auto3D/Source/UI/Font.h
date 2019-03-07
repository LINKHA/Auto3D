#include "../Resource/Resource.h"

namespace Auto3D
{

class FontFace;

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
	~Font();
	/// Register object factory.
	static void RegisterObject();

	/// Load resource from stream. May be called from a worker thread. Return true if successful.
	bool BeginLoad(Stream& source) override;
	/// Set absolute (in pixels) position adjustment for glyphs.
	void SetAbsoluteGlyphOffset(const Vector2I& offset);
	/// Set point size scaled position adjustment for glyphs.
	void SetScaledGlyphOffset(const Vector2F& offset);

	/// Return font face. Pack and render to a texture if not rendered yet. Return null on error.
	FontFace* GetFace(float pointSize);

	/// Return font type.
	FontType GetFontType() const { return fontType_; }

	/// Is signed distance field font.
	bool IsSDFFont() const { return sdfFont_; }

	/// Return absolute position adjustment for glyphs.
	const Vector2I& GetAbsoluteGlyphOffset() const { return absoluteOffset_; }

	/// Return point size scaled position adjustment for glyphs.
	const Vector2F& GetScaledGlyphOffset() const { return scaledOffset_; }

	/// Return the total effective offset for a point size.
	Vector2I GetTotalGlyphOffset(float pointSize) const;

	/// Release font faces and recreate them next time when requested. Called when font textures lost or global font properties change.
	void ReleaseFaces();

private:
	/// Return font face using FreeType. Called internally. Return null on error.
	FontFace* GetFaceFreeType(float pointSize);
	/// Return bitmap font face. Called internally. Return null on error.
	FontFace* GetFaceBitmap(float pointSize);

	/// Created faces.
	HashMap<int, SharedPtr<FontFace> > faces_;
	/// Font data.
	SharedArrayPtr<unsigned char> fontData_;
	/// Size of font data.
	unsigned fontDataSize_;
	/// Absolute position adjustment for glyphs.
	Vector2I absoluteOffset_;
	/// Point size scaled position adjustment for glyphs.
	Vector2F scaledOffset_;
	/// Font type.
	FontType fontType_;
	/// Signed distance field font flag.
	bool sdfFont_;
};

}