#pragma once
#include "../Container/String.h"
#include "../Container/Vector.h"

namespace Auto3D
{

bool OpenFileDialog(const String& filterList, const String& defaultPath, String& outPath);
bool OpenMultipleFilesDialog(const String& filterList, const String& defaultPath,
								Vector<String>& outPaths);
bool PickFolderDialog(const String& defaultPath, String& outPath);
bool SaveFileDialog(const String& filterList, const String& defaultPath, String& outPath);
}
