#pragma once

#include "../Base/Ptr.h"
#include "../Base/List.h"
#include "../Base/HashMap.h"

#include "../Math/AreaAllocator.h"


namespace Auto3D
{

class Font;
class Image;
class Texture;

/// Font glyph description.
struct AUTO_API FontGlyph
{
	/// X position in texture.
	short x_{};
	/// Y position in texture.
	short y_{};
	/// Width in texture.
	short texWidth_{};
	/// Height in texture.
	short texHeight_{};
	/// Width on screen.
	float width_{};
	/// Height on screen.
	float height_{};
	/// Glyph X offset from origin.
	float offsetX_{};
	/// Glyph Y offset from origin.
	float offsetY_{};
	/// Horizontal advance.
	float advanceX_{};
	/// Texture page. M_MAX_UNSIGNED if not yet resident on any texture.
	unsigned page_{ M_MAX_UNSIGNED };
	/// Used flag.
	bool used_{};
};

/// %Font face description.
class AUTO_API FontFace : public RefCounted
{
	friend class Font;

public:
	/// Construct.
	explicit FontFace(Font* font);
	/// Destruct.
	~FontFace();

	/// Load font face.
	virtual bool Load(const unsigned char* fontData, unsigned fontDataSize, float pointSize) = 0;
	/// Return pointer to the glyph structure corresponding to a character. Return null if glyph not found.
	virtual const FontGlyph* GetGlyph(unsigned c);

	/// Return if font face uses mutable glyphs.
	virtual bool HasMutableGlyphs() const { return false; }

	/// Return the kerning for a character and the next character.
	float GetKerning(unsigned c, unsigned d) const;
	/// Return true when one of the texture has a data loss.
	bool IsDataLost() const;

	/// Return point size.
	float GetPointSize() const { return pointSize_; }

	/// Return row height.
	float GetRowHeight() const { return rowHeight_; }

	/// Return textures.
	const Vector<SharedPtr<Texture> >& GetTextures() const { return textures_; }

protected:
	friend class FontFaceBitmap;
	/// Create a texture for font rendering.
	SharedPtr<Texture> CreateFaceTexture();
	/// Load font face texture from image resource.
	SharedPtr<Texture> LoadFaceTexture(const SharedPtr<Image>& image);

	/// Parent font.
	Font* font_{};
	/// Glyph mapping.
	HashMap<unsigned, FontGlyph> glyphMapping_;
	/// Kerning mapping.
	HashMap<unsigned, float> kerningMapping_;
	/// Glyph texture pages.
	Vector<SharedPtr<Texture> > textures_;
	/// Point size.
	float pointSize_{};
	/// Row height.
	float rowHeight_{};
};

}
