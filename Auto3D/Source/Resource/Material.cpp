#include "Resource/Material.h"
#include "IO/JSONFile.h"
#include "IO/JSONValue.h"
#include "Renderer/Pass.h"

namespace Auto3D
{

OMaterial::OMaterial()
{
	_pass = MakeShared<FPass>();
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

	if (root.Contains("shaders"))
	{
		
		const JSONObject& jsonShader = root["shaders"].GetObject();

		FShaderProgram& shaderProgram = _pass->_shaderProgram;

		if (jsonShader.Contains("vs"))
		{
			shaderProgram.CreateVertexShader(jsonShader.Find("vs")->_second.GetString());
		}

		if (jsonShader.Contains("ps"))
		{
			shaderProgram.CreatePixelShader(jsonShader.Find("ps")->_second.GetString());
		}

		shaderProgram.Link();
	}

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