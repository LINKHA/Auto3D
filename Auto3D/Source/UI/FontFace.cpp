#include "FontFace.h"
#include "Font.h"

#include "../Debug/Log.h"
#include "../Graphics/Texture.h"
#include "../Resource/Image.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

FontFace::FontFace(Font* font) :
	font_(font)
{
}

FontFace::~FontFace()
{
	if (font_)
	{
		// When a face is unloaded, deduct the used texture data size from the parent font
		unsigned totalTextureSize = 0;
		for (unsigned i = 0; i < textures_.Size(); ++i)
			totalTextureSize += textures_[i]->GetWidth() * textures_[i]->GetHeight();
		font_->SetMemoryUse(font_->GetMemoryUse() - totalTextureSize);
	}
}

const FontGlyph* FontFace::GetGlyph(unsigned c)
{
	HashMap<unsigned, FontGlyph>::Iterator i = glyphMapping_.Find(c);
	if (i != glyphMapping_.End())
	{
		FontGlyph& glyph = i->second;
		glyph.used_ = true;
		return &glyph;
	}
	else
		return nullptr;
}

float FontFace::GetKerning(unsigned c, unsigned d) const
{
	if (kerningMapping_.IsEmpty())
		return 0;

	if (c == '\n' || d == '\n')
		return 0;

	if (c > 0xffff || d > 0xffff)
		return 0;

	unsigned value = (c << 16u) + d;

	HashMap<unsigned, float>::ConstIterator i = kerningMapping_.Find(value);
	if (i != kerningMapping_.End())
		return i->second;

	return 0;
}

bool FontFace::IsDataLost() const
{
	for (unsigned i = 0; i < textures_.Size(); ++i)
	{
		if (textures_[i]->IsDataLost())
			return true;
	}
	return false;
}


SharedPtr<Texture> FontFace::CreateFaceTexture()
{
	SharedPtr<Texture> texture(new Texture());

	return texture;
}

SharedPtr<Texture> FontFace::LoadFaceTexture(const SharedPtr<Image>& image)
{
	SharedPtr<Texture> texture = CreateFaceTexture();

	return texture;
}

}
