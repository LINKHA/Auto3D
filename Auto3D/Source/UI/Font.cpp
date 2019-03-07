#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../IO/Stream.h"
#include "../Graphics/Graphics.h"
#include "../IO/FileSystem.h"
#include "../UI/Font.h"
#include "../UI/FontFaceFreeType.h"
#include "../Resource/ResourceCache.h"
#include "../Graphics/Graphics.h"


#include "../Debug/DebugNew.h"

namespace Auto3D
{

namespace
{
/// Convert float to 26.6 fixed-point (as used internally by FreeType)
inline int FloatToFixed(float value)
{
	return (int)(value * 64);
}

}

static const float MIN_POINT_SIZE = 1;
static const float MAX_POINT_SIZE = 96;

Font::Font() :
	fontDataSize_(0),
	absoluteOffset_(Vector2I::ZERO),
	scaledOffset_(Vector2F::ZERO),
	fontType_(FONT_NONE),
	sdfFont_(false)
{
}

Font::~Font()
{
	// To ensure FreeType deallocates properly, first clear all faces, then release the raw font data
	ReleaseFaces();
	fontData_.Reset();
}

void Font::RegisterObject()
{
	RegisterFactory<Font>();
}

bool Font::BeginLoad(Stream& source)
{
	// In headless mode, do not actually load, just return success
	auto graphics = Subsystem<Graphics>();

	if (!graphics)
		return true;

	fontType_ = FONT_NONE;
	faces_.Clear();

	fontDataSize_ = source.Size();
	if (fontDataSize_)
	{
		fontData_ = new unsigned char[fontDataSize_];
		if (source.Read(&fontData_[0], fontDataSize_) != fontDataSize_)
			return false;
	}
	else
	{
		fontData_.Reset();
		return false;
	}

	String ext = Extension(Name());
	if (ext == ".ttf" || ext == ".otf" || ext == ".woff")
	{
		fontType_ = FONT_FREETYPE;
		//LoadParameters();
	}
	else if (ext == ".xml" || ext == ".fnt" || ext == ".sdf")
		fontType_ = FONT_BITMAP;

	sdfFont_ = ext == ".sdf";

	SetMemoryUse(fontDataSize_);
	return true;
}

void Font::SetAbsoluteGlyphOffset(const Vector2I& offset)
{
	absoluteOffset_ = offset;
}

void Font::SetScaledGlyphOffset(const Vector2F& offset)
{
	scaledOffset_ = offset;
}

FontFace* Font::GetFace(float pointSize)
{
	// In headless mode, always return null
	auto graphics = Subsystem<Graphics>();
	if (!graphics)
		return nullptr;

	// For bitmap font type, always return the same font face provided by the font's bitmap file regardless of the actual requested point size
	if (fontType_ == FONT_BITMAP)
		pointSize = 0;
	else
		pointSize = Clamp(pointSize, MIN_POINT_SIZE, MAX_POINT_SIZE);

	// For outline fonts, we return the nearest size in 1/64th increments, as that's what FreeType supports.
	int key = FloatToFixed(pointSize);
	HashMap<int, SharedPtr<FontFace> >::Iterator i = faces_.Find(key);
	if (i != faces_.End())
	{
		if (!i->second->IsDataLost())
			return i->second;
		else
		{
			// Erase and reload face if texture data lost (OpenGL mode only)
			faces_.Erase(i);
		}
	}

	PROFILE(GetFontFace);

	switch (fontType_)
	{
	case FONT_FREETYPE:
		return GetFaceFreeType(pointSize);

	case FONT_BITMAP:
		return GetFaceBitmap(pointSize);

	default:
		return nullptr;
	}
}

Vector2I Font::GetTotalGlyphOffset(float pointSize) const
{
	Vector2F multipliedOffset = pointSize * scaledOffset_;
	return absoluteOffset_ + Vector2I(RoundToInt(multipliedOffset._x), RoundToInt(multipliedOffset._y));
}

void Font::ReleaseFaces()
{
	faces_.Clear();
}

FontFace* Font::GetFaceFreeType(float pointSize)
{
	SharedPtr<FontFace> newFace(new FontFaceFreeType(this));
	if (!newFace->Load(&fontData_[0], fontDataSize_, pointSize))
		return nullptr;

	int key = FloatToFixed(pointSize);
	faces_[key] = newFace;
	return newFace;
}



}
