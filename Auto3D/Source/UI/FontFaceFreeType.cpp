#include "FontFaceFreeType.h"
#include "../Object/Object.h"
#include "../Debug/Log.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H

namespace Auto3D
{

/// FreeType library subsystem.
class FreeTypeLibrary : public Object
{
	REGISTER_OBJECT_CLASS(FreeTypeLibrary, Object)
public:
	/// Construct.
	FreeTypeLibrary()
	{
		FT_Error error = FT_Init_FreeType(&library_);
		if (error)
			ErrorString("Could not initialize FreeType library");
	}

	/// Destruct.
	~FreeTypeLibrary() override
	{
		FT_Done_FreeType(library_);
	}

	FT_Library GetLibrary() const { return library_; }

private:
	/// FreeType library.
	FT_Library library_{};
};

FontFaceFreeType::FontFaceFreeType()
{
	new FreeTypeLibrary();
}
FontFaceFreeType::~FontFaceFreeType()
{

}
	
}