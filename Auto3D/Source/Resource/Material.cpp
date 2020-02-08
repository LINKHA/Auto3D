#include "Resource/Material.h"
#include "IO/JSONFile.h"
#include "IO/JSONValue.h"
#include "Renderer/Pass.h"

namespace Auto3D
{
SPtr<OMaterial> OMaterial::_defaultMaterial;

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

	if (root.Contains("shaders"))
	{
		
		const JSONObject& jsonShader = root["shaders"].GetObject();

		if (jsonShader.Contains("vs"))
		{
			_shaderProgram.CreateVertexShader(jsonShader.Find("vs")->_second.GetString());
		}

		if (jsonShader.Contains("vsi"))
		{
			_shaderInstanceProgram.CreateVertexShader(jsonShader.Find("vsi")->_second.GetString());
		}

		if (jsonShader.Contains("ps"))
		{
			_shaderProgram.CreatePixelShader(jsonShader.Find("ps")->_second.GetString());
			_shaderInstanceProgram.CreatePixelShader(jsonShader.Find("ps")->_second.GetString());
		}

		_shaderProgram.Link();
		_shaderInstanceProgram.Link();
	}

	if (root.Contains("textures"))
	{
		const JSONObject& jsonTextures = root["textures"].GetObject();
		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
		{
			
		}
	}

	return true;
}

bool OMaterial::EndLoad()
{
	return true;
}

FShaderProgram& OMaterial::GetShaderProgram()
{
	return _shaderProgram;
}
FShaderProgram& OMaterial::GetShaderInstanceProgram()
{
	return _shaderInstanceProgram;
}
OMaterial* OMaterial::DefaultMaterial()
{
	// Create on demand
	if (!_defaultMaterial)
	{
		/*	_defaultMaterial = new AMaterial();
			FPass* pass = _defaultMaterial->CreatePass("opaque");
			pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");

			pass = _defaultMaterial->CreatePass("opaqueadd");
			pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");
			pass->SetBlendMode(EBlendMode::ADD);
			pass->_depthWrite = false;

			pass = _defaultMaterial->CreatePass("shadow");
			pass->SetShaders("Shader/Shadow", "Shader/Shadow");
			pass->_colorWriteMask = COLORMASK_NONE;*/
	}

	return _defaultMaterial.get();
}

}