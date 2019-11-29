#pragma once
#include "Container/String.h"
#include "Container/Vector.h"

namespace Auto3D
{

bool OpenFileDialog(const FString& filterList, const FString& defaultPath, FString& outPath);
bool OpenMultipleFilesDialog(const FString& filterList, const FString& defaultPath,
								TVector<FString>& outPaths);
bool PickFolderDialog(const FString& defaultPath, FString& outPath);
bool SaveFileDialog(const FString& filterList, const FString& defaultPath, FString& outPath);
}
