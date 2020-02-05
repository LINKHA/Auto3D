#include "Resource/Material.h"
#include "IO/JSONFile.h"

namespace Auto3D
{

OMaterial::OMaterial()
{

}

OMaterial::~OMaterial()
{

}

bool OMaterial::BeginLoad(const FString& pathName)
{
	_loadJSON = MakeUnique<OJSONFile>();
	if (!_loadJSON->Load(pathName))
		return false;

	const FJSONValue& root = _loadJSON->Root();

	if (root.Contains("textures"))
	{
		const JSONObject& jsonTextures = root["textures"].GetObject();
		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
		{
			
		}
	}
}

bool OMaterial::EndLoad()
{
	return true;
}

}