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

AFont::AFont() :
	_fontDataSize(0),
	_absoluteOffset(Vector2I::ZERO),
	_scaledOffset(Vector2F::ZERO),
	_fontType(EFontType::NONE),
	_sdfFont(false)
{
}

AFont::~AFont()
{
	_fontData.Reset();
}

void AFont::RegisterObject()
{
	RegisterFactory<AFont>();
}

bool AFont::BeginLoad(FStream& source)
{
	// In headless mode, do not actually load, just return success
	AGraphics* graphics = GModuleManager::Get().GraphicsModule();
	if (!graphics)
		return true;

	_fontType = EFontType::NONE;

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

	FString ext = Extension(FName());
	if (ext == ".ttf" || ext == ".otf" || ext == ".woff")
	{
		_fontType = EFontType::FREE_TYPE;
	}
	else if (ext == ".xml" || ext == ".fnt" || ext == ".sdf")
		_fontType = EFontType::BIT_MAP;

	_sdfFont = ext == ".sdf";

	SetMemoryUse(_fontDataSize);
	return true;
}
void AFont::SetAbsoluteGlyphOffset(const Vector2I& offset)
{
	_absoluteOffset = offset;
}

void AFont::SetScaledGlyphOffset(const Vector2F& offset)
{
	_scaledOffset = offset;
}


Vector2I AFont::GetTotalGlyphOffset(float pointSize) const
{
	Vector2F multipliedOffset = pointSize * _scaledOffset;
	return _absoluteOffset + Vector2I(RoundToInt(multipliedOffset._x), RoundToInt(multipliedOffset._y));
}


}
