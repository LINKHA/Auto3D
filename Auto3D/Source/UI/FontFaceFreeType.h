#pragma once
#include "FontFace.h"
#include "../AutoConfig.h"

namespace Auto3D
{

class FreeTypeLibrary;
class Texture;

class AUTO_API FontFaceFreeType : public FontFace
{
public:
	/// Construct.
	explicit FontFaceFreeType(Font* font);
	/// Destruct.
	~FontFaceFreeType() override;
	/// Load font face.
	bool Load(const unsigned char* fontData, unsigned fontDataSize, float pointSize) override;
private:
	/// FreeType library.
	SharedPtr<FreeTypeLibrary> freeType_;
	/// FreeType face. Non-null after creation only in dynamic mode.
	void* face_{};
	/// Load mode.
	int loadMode_{};
	/// Use subpixel glyph positioning?
	bool subpixel_{};
	/// Oversampling level.
	int oversampling_{};
	/// Ascender.
	float ascender_{};
	/// Has mutable glyph.
	bool hasMutableGlyph_{};
	/// Glyph area allocator.
	AreaAllocator allocator_;
};


}