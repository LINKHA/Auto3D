#include "Font.h"

#include "Graphics/Graphics.h"
#include "Core/Modules/ModuleManager.h"
#include "IO/FileSystem.h"
#include "IO/Stream.h"

#include "Debug/DebugNew.h"

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
	_fontDataSize(0),
	_absoluteOffset(Vector2I::ZERO),
	_scaledOffset(Vector2F::ZERO),
	_fontType(FONT_NONE),
	_sdfFont(false)
{
}

Font::~Font()
{
	_fontData.Reset();
}

void Font::RegisterObject()
{
	RegisterFactory<Font>();
}

bool Font::BeginLoad(Stream& source)
{
	// In headless mode, do not actually load, just return success
	Graphics* graphics = ModuleManager::Get().GraphicsModule();
	if (!graphics)
		return true;

	_fontType = FONT_NONE;

	_fontDataSize = source.Size();
	if (_fontDataSize)
	{
		_fontData = new unsigned char[_fontDataSize];
		if (source.Read(&_fontData[0], _fontDataSize) != _fontDataSize)
			return false;
	}
	else
	{
		_fontData.Reset();
		return false;
	}

	FString ext = Extension(Name());
	if (ext == ".ttf" || ext == ".otf" || ext == ".woff")
	{
		_fontType = FONT_FREETYPE;
	}
	else if (ext == ".xml" || ext == ".fnt" || ext == ".sdf")
		_fontType = FONT_BITMAP;

	_sdfFont = ext == ".sdf";

	SetMemoryUse(_fontDataSize);
	return true;
}
void Font::SetAbsoluteGlyphOffset(const Vector2I& offset)
{
	_absoluteOffset = offset;
}

void Font::SetScaledGlyphOffset(const Vector2F& offset)
{
	_scaledOffset = offset;
}


Vector2I Font::GetTotalGlyphOffset(float pointSize) const
{
	Vector2F multipliedOffset = pointSize * _scaledOffset;
	return _absoluteOffset + Vector2I(RoundToInt(multipliedOffset._x), RoundToInt(multipliedOffset._y));
}


}
