#include "FontFaceFreeType.h"
#include "Font.h"
#include "../Object/GameManager.h"
#include "../Debug/Log.h"

#include "../Debug/DebugNew.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H

namespace Auto3D
{

/// FreeType library subsystem.
class FreeTypeLibrary : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(FreeTypeLibrary, BaseSubsystem)
public:
	/// Construct.
	FreeTypeLibrary()
	{
		FT_Error error = FT_Init_FreeType(&_library);
		if (error)
			ErrorString("Could not initialize FreeType library");
	}

	/// Destruct.
	~FreeTypeLibrary() override
	{
		FT_Done_FreeType(_library);
	}

	FT_Library GetLibrary() const { return _library; }

private:
	/// FreeType library.
	FT_Library _library{};
};


FontFaceFreeType::FontFaceFreeType(Font* font) :
	FontFace(font),
	loadMode_(FT_LOAD_DEFAULT)
{
}

FontFaceFreeType::~FontFaceFreeType()
{
	if (face_)
	{
		FT_Done_Face((FT_Face)face_);
		face_ = nullptr;
	}
}

bool FontFaceFreeType::Load(const unsigned char* fontData, unsigned fontDataSize, float pointSize)
{
	// Create & initialize FreeType library if it does not exist yet
	auto freeType = font_->Subsystem<FreeTypeLibrary>();
	if (!freeType)
		font_->RegisterSubsystem(freeType = new FreeTypeLibrary());

	return false;
}
	
}