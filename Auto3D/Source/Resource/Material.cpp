#include "Resource/Material.h"
#include "IO/JSONFile.h"
#include "IO/JSONValue.h"
#include "Renderer/Pass.h"
#include "Debug/Log.h"

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

		bool shaderFlag = false;
		bool shaderInstanceFlag = false;

		if (jsonShader.Contains("vs"))
		{
			_shaderProgram.CreateVertexShader(jsonShader.Find("vs")->_second.GetString());
			shaderFlag = true;
		}

		if (jsonShader.Contains("vsi"))
		{
			_shaderInstanceProgram.CreateVertexShader(jsonShader.Find("vsi")->_second.GetString());
			shaderInstanceFlag = true;
		}

		if (jsonShader.Contains("ps"))
		{
			if(shaderFlag)
				_shaderProgram.CreatePixelShader(jsonShader.Find("ps")->_second.GetString());
			if(shaderInstanceFlag)
				_shaderInstanceProgram.CreatePixelShader(jsonShader.Find("ps")->_second.GetString());
		}

		if (jsonShader.Contains("uniforms"))
		{
			_uniforms.Clear();

			const JSONObject& uniforms = root["shaders"]["uniforms"].GetObject();
			for (auto it = uniforms.Begin(); it != uniforms.End(); ++it)
			{
				FString uniformType = it->_first;
				const FJSONValue& jsonValue = it->_second;
				bgfx::UniformHandle uniformHandle = BGFX_INVALID_HANDLE;

				if (uniformType == UNIFORM_SAMPLER)
					uniformHandle = bgfx::createUniform(jsonValue.GetString().CString(), bgfx::UniformType::Sampler);
				else if (uniformType == UNIFORM_END)
					uniformHandle = bgfx::createUniform(jsonValue.GetString().CString(), bgfx::UniformType::End);
				else if (uniformType == UNIFORM_VEC4)
					uniformHandle = bgfx::createUniform(jsonValue.GetString().CString(), bgfx::UniformType::Vec4);
				else if (uniformType == UNIFORM_MAT3)
					uniformHandle = bgfx::createUniform(jsonValue.GetString().CString(), bgfx::UniformType::Mat3);
				else if (uniformType == UNIFORM_MAT4)
					uniformHandle = bgfx::createUniform(jsonValue.GetString().CString(), bgfx::UniformType::Mat4);
				else
				{
					ErrorString("Fail set uniform.");
					continue;
				}

				_uniforms[uniformType] = uniformHandle;
			}
		}
		if(shaderFlag)
			_shaderProgram.Link();
		if(shaderInstanceFlag)
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

THashMap<FString, bgfx::UniformHandle>& OMaterial::GetUniforms()
{
	return _uniforms;
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