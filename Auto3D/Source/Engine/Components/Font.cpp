#include "Font.h"

#include "Graphics/Graphics.h"
#include "Core/Modules/ModuleManager.h"
#include "IO/FileSystem.h"
#include "IO/Stream.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static const int FONT_TEXTURE_MIN_SIZE = 128;
static const int FONT_DPI = 96;

static const float MIN_POINT_SIZE = 1;
static const float MAX_POINT_SIZE = 96;

namespace
{
	/// Convert float to 26.6 fixed-point (as used internally by FreeType)
	inline int FloatToFixed(float value)
	{
		return (int)(value * 64);
	}
}

// Font cpp fail,Initializes the global constructor so apply here
// Apply in Resource CPP
//REGISTER_CLASS
//{
//	REGISTER_CALSS_FACTORY_IMP(AFont)
//	.constructor<>()
//	.property_readonly("sdfFont",&AFont::IsSDFFont)
//	;
//}

AFont::AFont() :
	_fontDataSize(0),
	_absoluteOffset(TVector2I::ZERO),
	_scaledOffset(TVector2F::ZERO),
	_fontType(EFontType::NONE),
	_sdfFont(false)
{
}

AFont::~AFont()
{
	_fontData.Reset();
}

bool AFont::BeginLoad(FStream& source)
{
	// In headless mode, do not actually load, just return success
	FGraphicsModule* graphics = GModuleManager::Get().GraphicsModule();
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

	FString ext = Extension(GetName());
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
void AFont::SetAbsoluteGlyphOffset(const TVector2I& offset)
{
	_absoluteOffset = offset;
}

void AFont::SetScaledGlyphOffset(const TVector2F& offset)
{
	_scaledOffset = offset;
}


TVector2I AFont::GetTotalGlyphOffset(float pointSize) const
{
	TVector2F multipliedOffset = pointSize * _scaledOffset;
	return _absoluteOffset + TVector2I(RoundToInt(multipliedOffset._x), RoundToInt(multipliedOffset._y));
}


}
