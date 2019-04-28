#include "PBRMaterial.h"

#include "../Debug/Profiler.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ShaderVariation.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Resource/JSONFile.h"
#include "../Resource/ResourceCache.h"

namespace Auto3D
{

PBRMaterial::PBRMaterial() :
	_metallic(0.0f),
	_roughness(0.0f),
	_ao(0.0f)
{
}

PBRMaterial::~PBRMaterial() = default;

void PBRMaterial::RegisterObject()
{
	RegisterFactory<PBRMaterial>();
}

bool PBRMaterial::EndLoad()
{
	PROFILE(EndLoadMaterial);

	const JSONValue& root = _loadJSON->Root();

	_passes.Clear();
	if (root.Contains("passes"))
	{
		const JSONObject& jsonPasses = root["passes"].GetObject();
		for (auto it = jsonPasses.Begin(); it != jsonPasses.End(); ++it)
		{
			Pass* newPass = CreatePass(it->_first);
			newPass->LoadJSON(it->_second);
		}
	}

	_constantBuffers[ShaderStage::VS].Reset();
	if (root.Contains("vsConstantBuffer"))
	{
		_constantBuffers[ShaderStage::VS] = new ConstantBuffer();
		_constantBuffers[ShaderStage::VS]->LoadJSON(root["vsConstantBuffer"].GetObject());
	}

	_constantBuffers[ShaderStage::PS].Reset();
	if (root.Contains("psConstantBuffer"))
	{
		_constantBuffers[ShaderStage::PS] = new ConstantBuffer();
		_constantBuffers[ShaderStage::PS]->LoadJSON(root["psConstantBuffer"].GetObject());
	}

	/// \todo Queue texture loads during BeginLoad()
	ResetTextures();
	if (root.Contains("textures"))
	{
		ResourceCache* cache = Subsystem<ResourceCache>();
		const JSONObject& jsonTextures = root["textures"].GetObject();
		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
			SetTexture(it->_first.ToInt(), cache->LoadResource<Texture>(it->_second.GetString()));
	}
	if (root.Contains("texturesMap"))
	{
		ResourceCache* cache = Subsystem<ResourceCache>();
		const JSONObject& jsonTextures = root["texturesMap"].GetObject();
		Vector<Image*> imageData;
		Vector<ImageLevel> faces;

		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
			imageData.Push(cache->LoadResource<Image>(it->_second.GetString()));


		for (int i = 0; i < MAX_CUBE_FACES; ++i)
		{
			faces.Push(imageData[i]->GetLevel(0));
		}

		Texture* textureCube = new Texture();
		textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, imageData[0]->GetLevel(0)._size, imageData[0]->GetFormat(), 1, &faces[0]);
		textureCube->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
		textureCube->SetDataLost(false);
		SetTexture(jsonTextures.Begin()->_first.ToInt(), textureCube);

	}
	if (root.Contains("PBR"))
	{
		const JSONObject& jsonTextures = root["PBR"].GetObject();
		this->LoadJSON(jsonTextures);
	}

	_loadJSON.Reset();
	return true;

}

void PBRMaterial::LoadJSON(const JSONValue& source)
{
	//if (source.Contains("albedo"))
	//	_albedo = source["albedo"].GetVec3();
	if (source.Contains("metallic"))
		_metallic = source["metallic"].GetNumber();
	if (source.Contains("roughness"))
		_roughness = source["roughness"].GetNumber();
	if (source.Contains("ao"))
		_ao = source["ao"].GetNumber();
}

}