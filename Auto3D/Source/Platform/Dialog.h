#include "Container/String.h"

namespace bx { class FilePath; class StringView; }

namespace Auto3D
{

struct FileSelectionDialogType
{
	enum Enum
	{
		Open,
		Save,

		Count
	};
};

///
bool OpenFileSelectionDialog(
	bx::FilePath& _inOutFilePath
	, FileSelectionDialogType::Enum _type
	, const bx::StringView& _title
	, const bx::StringView& _filter = "All Files | *"
);

///
void OpenUrl(const FString& url);

}