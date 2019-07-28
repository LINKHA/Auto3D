#include "AutoFileDialog.h"
#include <filedialog/nfd/nfd.h>
#include <filedialog/nfd/nfd_common.h>

namespace Auto3D
{

bool OpenFileDialog(const String& filterList, const String& defaultPath, String& outPath)
{
	nfdchar_t* out = nullptr;
	nfdresult_t result = NFD_OpenDialog(filterList.CString(), defaultPath.CString(), &out);
	if(result == NFD_OKAY)
	{
		outPath = out;
		NFDi_Free(out);
		return true;
	}

	return false;
}

bool OpenMultipleFilesDialog(const String& filterList, const String& defaultPath,
								Vector<String>& outPaths)
{
	nfdpathset_t out;
	nfdresult_t result = NFD_OpenDialogMultiple(filterList.CString(), defaultPath.CString(), &out);
	if(result == NFD_OKAY)
	{
		auto selected_count = NFD_PathSet_GetCount(&out);
		for(size_t i = 0; i < selected_count; ++i)
		{
			nfdchar_t* path = NFD_PathSet_GetPath(&out, i);
			outPaths.Push(path);
		}
		NFD_PathSet_Free(&out);
		return true;
	}

	return false;
}

bool PickFolderDialog(const String& defaultPath, String& outPath)
{
	nfdchar_t* out = nullptr;
	nfdresult_t result = NFD_PickFolder(defaultPath.CString(), &out);
	if(result == NFD_OKAY)
	{
		outPath = out;
		NFDi_Free(out);
		return true;
	}

	return false;
}

bool SaveFileDialog(const String& filterList, const String& defaultPath, String& outPath)
{
	nfdchar_t* out = nullptr;
	nfdresult_t result = NFD_SaveDialog(filterList.CString(), defaultPath.CString(), &out);
	if(result == NFD_OKAY)
	{
		outPath = String(out);
		NFDi_Free(out);
		return true;
	}

	return false;
}
}
