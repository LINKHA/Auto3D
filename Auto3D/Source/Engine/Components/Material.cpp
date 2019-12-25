#include "Debug/Profiler.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/ShaderVariation.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Resource/JSONFile.h"
#include "Resource/ResourceCache.h"
#include "Engine/Components/IBLMaterial.h"

#include "Material.h"

#include "Core/Modules/ModuleManager.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

TSharedPtr<AMaterial> AMaterial::_defaultMaterial;
THashMap<FString, unsigned char> AMaterial::_passIndices;
TVector<FString> AMaterial::_passNames;
unsigned char AMaterial::_nextPassIndex = 0;

FPass::FPass(AMaterial* parent, const FString& name) :
    _parent(parent),
    _name(name),
    _shaderHash(0),
    _shadersLoaded(false)
{
    Reset();
}

FPass::~FPass()
{
}

bool FPass::LoadJSON(const FJSONValue& source)
{
    if (source.Contains("vs"))
        _shaderNames[EShaderStage::VS] = source["vs"].GetString();
    if (source.Contains("ps"))
        _shaderNames[EShaderStage::PS] = source["ps"].GetString();
    if (source.Contains("vsDefines"))
        _shaderDefines[EShaderStage::VS] = source["vsDefines"].GetString();
    if (source.Contains("psDefines"))
        _shaderDefines[EShaderStage::PS] = source["psDefines"].GetString();

    if (source.Contains("depthFunc"))
        _depthFunc = (ECompareFunc::Type)FString::ListIndex(source["depthFunc"].GetString(), compareFuncNames, ECompareFunc::LESS_EQUAL);
    if (source.Contains("depthWrite"))
        _depthWrite = source["depthWrite"].GetBool();
    if (source.Contains("depthClip"))
        _depthClip = source["depthClip"].GetBool();
    if (source.Contains("alphaToCoverage"))
        _alphaToCoverage = source["alphaToCoverage"].GetBool();
    if (source.Contains("colorWriteMask"))
        _colorWriteMask = (unsigned char)source["colorWriteMask"].GetNumber();
    if (source.Contains("blendMode"))
        _blendMode = blendModes[FString::ListIndex(source["blendMode"].GetString(), blendModeNames, EBlendMode::REPLACE)];
    else
    {
        if (source.Contains("blendEnable"))
            _blendMode._blendEnable = source["blendEnable"].GetBool();
        if (source.Contains("srcBlend"))
            _blendMode._srcBlend = (EBlendFactor::Type)FString::ListIndex(source["srcBlend"].GetString(), blendFactorNames, EBlendFactor::ONE);
        if (source.Contains("destBlend"))
            _blendMode._destBlend = (EBlendFactor::Type)FString::ListIndex(source["destBlend"].GetString(), blendFactorNames, EBlendFactor::ONE);
        if (source.Contains("blendOp"))
            _blendMode._blendOp = (EBlendOp::Type)FString::ListIndex(source["blendOp"].GetString(), blendOpNames, EBlendOp::ADD);
        if (source.Contains("srcBlendAlpha"))
            _blendMode._srcBlendAlpha = (EBlendFactor::Type)FString::ListIndex(source["srcBlendAlpha"].GetString(), blendFactorNames, EBlendFactor::ONE);
        if (source.Contains("destBlendAlpha"))
            _blendMode._destBlendAlpha = (EBlendFactor::Type)FString::ListIndex(source["destBlendAlpha"].GetString(), blendFactorNames, EBlendFactor::ONE);
        if (source.Contains("blendOpAlpha"))
            _blendMode._blendOpAlpha = (EBlendOp::Type)FString::ListIndex(source["blendOpAlpha"].GetString(), blendOpNames, EBlendOp::ADD);
    }

    if (source.Contains("fillMode"))
        _fillMode = (EFillMode::Type)FString::ListIndex(source["fillMode"].GetString(), fillModeNames, EFillMode::SOLID);
    if (source.Contains("cullMode"))
        _cullMode = (ECullMode::Type)FString::ListIndex(source["cullMode"].GetString(), cullModeNames, ECullMode::BACK);

    OnShadersChanged();
    return true;
}

bool FPass::SaveJSON(FJSONValue& dest)
{
    dest.SetEmptyObject();

    if (_shaderNames[EShaderStage::VS].Length())
        dest["vs"] = _shaderNames[EShaderStage::VS];
    if (_shaderNames[EShaderStage::PS].Length())
        dest["ps"] = _shaderNames[EShaderStage::PS];
    if (_shaderDefines[EShaderStage::VS].Length())
        dest["vsDefines"] = _shaderDefines[EShaderStage::VS];
    if (_shaderDefines[EShaderStage::PS].Length())
        dest["psDefines"] = _shaderDefines[EShaderStage::PS];

    dest["depthFunc"] = compareFuncNames[_depthFunc];
    dest["depthWrite"] = _depthWrite;
    dest["depthClip"] = _depthClip;
    dest["alphaToCoverage"] = _alphaToCoverage;
    dest["colorWriteMask"] = _colorWriteMask;

    // Prefer saving a predefined blend mode if possible for better readability
    bool blendModeFound = false;
    for (size_t i = 0; i < EBlendMode::Count; ++i)
    {
        if (_blendMode == blendModes[i])
        {
            dest["blendMode"] = blendModeNames[i];
            blendModeFound = true;
            break;
        }
    }

    if (!blendModeFound)
    {
        dest["blendEnable"] = _blendMode._blendEnable;
        dest["srcBlend"] = blendFactorNames[_blendMode._srcBlend];
        dest["destBlend"] = blendFactorNames[_blendMode._destBlend];
        dest["blendOp"] = blendOpNames[_blendMode._blendOp];
        dest["srcBlendAlpha"] = blendFactorNames[_blendMode._srcBlendAlpha];
        dest["destBlendAlpha"] = blendFactorNames[_blendMode._destBlendAlpha];
        dest["blendOpAlpha"] = blendOpNames[_blendMode._blendOpAlpha];
    }

    dest["fillMode"] = fillModeNames[_fillMode];
    dest["cullMode"] = cullModeNames[_cullMode];

    return true;
}

void FPass::SetBlendMode(EBlendMode::Type mode)
{
    _blendMode = blendModes[mode];
}

void FPass::SetShaders(const FString& vsName, const FString& psName, const FString& vsDefines, const FString& psDefines)
{
    _shaderNames[EShaderStage::VS] = vsName;
    _shaderNames[EShaderStage::PS] = psName;
    _shaderDefines[EShaderStage::VS] = vsDefines;
    _shaderDefines[EShaderStage::PS] = psDefines;
    OnShadersChanged();
}

void FPass::Reset()
{
    _depthFunc = ECompareFunc::LESS_EQUAL;
    _depthWrite = true;
    _depthClip = true;
    _alphaToCoverage = false;
    _colorWriteMask = COLORMASK_ALL;
    _blendMode.Reset();
    _cullMode = ECullMode::BACK;
    _fillMode = EFillMode::SOLID;
}

AMaterial* FPass::Parent() const
{
    return _parent;
}

void FPass::OnShadersChanged()
{
    // Reset existing variations
    for (size_t i = 0; i < EShaderStage::Count; ++i)
    {
        _shaders[i].Reset();
        _shaderVariations[i].Clear();
    }

    _shadersLoaded = false;

    // Combine and trim the shader defines
    for (size_t i = 0; i < EShaderStage::Count; ++i)
    {
        const FString& materialDefines = _parent->ShaderDefines((EShaderStage::Type)i);
        if (materialDefines.Length())
            _combinedShaderDefines[i] = (materialDefines.Trimmed() + " " + _shaderDefines[i]).Trimmed();
        else
            _combinedShaderDefines[i] = _shaderDefines[i].Trimmed();
    }

    _shaderHash = FStringHash(_shaderNames[EShaderStage::VS] + _shaderNames[EShaderStage::PS] + _combinedShaderDefines[EShaderStage::VS] +
        _combinedShaderDefines[EShaderStage::PS]).Value();
}

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AMaterial)
	.constructor<>()
	;

	REGISTER_CALSS_FACTORY_IMP(AIBLMaterial)
	.constructor<>()
	;
}

AMaterial::AMaterial()
{
}

AMaterial::~AMaterial()
{
}

bool AMaterial::BeginLoad(FStream& source)
{
    PROFILE(BeginLoadMaterial);

    _loadJSON = new AJSONFile();
    if (!_loadJSON->Load(source))
        return false;

    const FJSONValue& root = _loadJSON->Root();

    _shaderDefines[EShaderStage::VS].Clear();
    _shaderDefines[EShaderStage::PS].Clear();
    if (root.Contains("vsDefines"))
        _shaderDefines[EShaderStage::VS] = root["vsDefines"].GetString();
    if (root.Contains("psDefines"))
        _shaderDefines[EShaderStage::PS] = root["psDefines"].GetString();

    return true;
}

bool AMaterial::EndLoad()
{
    PROFILE(EndLoadMaterial);

    const FJSONValue& root = _loadJSON->Root();


    _passes.Clear();
    if (root.Contains("passes"))
    {
        const JSONObject& jsonPasses = root["passes"].GetObject();
        for (auto it = jsonPasses.Begin(); it != jsonPasses.End(); ++it)
        {
            FPass* newPass = CreatePass(it->_first);
            newPass->LoadJSON(it->_second);
        }
    }

    _constantBuffers[EShaderStage::VS].Reset();
    if (root.Contains("vsConstantBuffer"))
    {
        _constantBuffers[EShaderStage::VS] = new FConstantBuffer();
        _constantBuffers[EShaderStage::VS]->LoadJSON(root["vsConstantBuffer"].GetObject());
    }

    _constantBuffers[EShaderStage::PS].Reset();
    if (root.Contains("psConstantBuffer"))
    {
        _constantBuffers[EShaderStage::PS] = new FConstantBuffer();
        _constantBuffers[EShaderStage::PS]->LoadJSON(root["psConstantBuffer"].GetObject());
    }
    
    /// \todo Queue texture loads during BeginLoad()
    ResetTextures();

	FResourceModule* cache = GModuleManager::Get().CacheModule();

    if (root.Contains("textures"))
    {
        const JSONObject& jsonTextures = root["textures"].GetObject();
        for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
            SetTexture(it->_first.ToInt(), cache->LoadResource<ATexture>(it->_second.GetString()));
    }
	if (root.Contains("texturesMap"))
	{
		const JSONObject& jsonTextures = root["texturesMap"].GetObject();
		TVector<AImage*> imageData;
		TVector<FImageLevel> faces;

		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
			imageData.Push(cache->LoadResource<AImage>(it->_second.GetString()));

		
		for (int i = 0; i < MAX_CUBE_FACES; ++i)
		{
			faces.Push(imageData[i]->GetLevel(0));
		}

		TSharedPtr<ATexture>textureCube(new ATexture());
		textureCube->Define(ETextureType::TEX_CUBE, EResourceUsage::DEFAULT, imageData[0]->GetLevel(0)._size, imageData[0]->GetFormat(), 1, &faces[0]);
		textureCube->DefineSampler(ETextureFilterMode::COMPARE_TRILINEAR, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
		textureCube->SetDataLost(false);
		SetTexture(jsonTextures.Begin()->_first.ToInt(), textureCube);
	}
    _loadJSON.Reset();
    return true;
}

bool AMaterial::Save(FStream& dest)
{
    PROFILE(SaveMaterial);

    AJSONFile saveJSON;
    FJSONValue& root = saveJSON.Root();
    root.SetEmptyObject();

    if (_shaderDefines[EShaderStage::VS].Length())
        root["vsDefines"] = _shaderDefines[EShaderStage::VS];
    if (_shaderDefines[EShaderStage::PS].Length())
        root["psDefines"] = _shaderDefines[EShaderStage::PS];
    
    if (_passes.Size())
    {
        root["passes"].SetEmptyObject();
        for (auto it = _passes.Begin(); it != _passes.End(); ++it)
        {
            FPass* pass = *it;
            if (pass)
                pass->SaveJSON(root["passes"][pass->GetName()]);
        }
    }

    if (_constantBuffers[EShaderStage::VS])
        _constantBuffers[EShaderStage::VS]->SaveJSON(root["vsConstantBuffer"]);
    if (_constantBuffers[EShaderStage::PS])
        _constantBuffers[EShaderStage::PS]->SaveJSON(root["psConstantBuffer"]);

    root["textures"].SetEmptyObject();
    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
    {
        if (_textures[i])
            root["textures"][FString((int)i)] = _textures[i]->GetName();
    }

    return saveJSON.Save(dest);
}

FPass* AMaterial::CreatePass(const FString& name)
{
    size_t index = PassIndex(name);
    if (_passes.Size() <= index)
        _passes.Resize(index + 1);

    if (!_passes[index])
        _passes[index] = new FPass(this, name);
    
    return _passes[index];
}

void AMaterial::RemovePass(const FString& name)
{
    size_t index = PassIndex(name, false);
    if (index < _passes.Size())
        _passes[index].Reset();
}

void AMaterial::SetTexture(size_t index, ATexture* texture)
{
    if (index < MAX_MATERIAL_TEXTURE_UNITS)
        _textures[index] = texture;
}

void AMaterial::ResetTextures()
{
    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
        _textures[i].Reset();
}

void AMaterial::SetConstantBuffer(EShaderStage::Type stage, FConstantBuffer* buffer)
{
    if (stage < EShaderStage::Count)
        _constantBuffers[stage] = buffer;
}

void AMaterial::SetShaderDefines(const FString& vsDefines, const FString& psDefines)
{
    _shaderDefines[EShaderStage::VS] = vsDefines;
    _shaderDefines[EShaderStage::PS] = psDefines;
    
    for (auto it = _passes.Begin(); it != _passes.End(); ++it)
    {
        FPass* pass = *it;
        if (pass)
            pass->OnShadersChanged();
    }
}

FPass* AMaterial::FindPass(const FString& name) const
{
    return GetPass(PassIndex(name, false));
}

FPass* AMaterial::GetPass(unsigned char index) const
{
    return index < _passes.Size() ? _passes[index].Get() : nullptr;
}

ATexture* AMaterial::GetTexture(size_t index) const
{
    return index < MAX_MATERIAL_TEXTURE_UNITS ? _textures[index].Get() : nullptr;
}

FConstantBuffer* AMaterial::GetConstantBuffer(EShaderStage::Type stage) const
{
    return stage < EShaderStage::Count ? _constantBuffers[stage].Get() : nullptr;
}

const FString& AMaterial::ShaderDefines(EShaderStage::Type stage) const
{
    return stage < EShaderStage::Count ? _shaderDefines[stage] : FString::EMPTY;
}

unsigned char AMaterial::PassIndex(const FString& name, bool createNew)
{
    FString nameLower = name.ToLower();
    auto it = _passIndices.Find(nameLower);
    if (it != _passIndices.End())
        return it->_second;
    else
    {
        if (createNew)
        {
            _passIndices[nameLower] = _nextPassIndex;
            _passNames.Push(nameLower);
            return _nextPassIndex++;
        }
        else
            return 0xff;
    }
}

const FString& AMaterial::PassName(unsigned char index)
{
    return index < _passNames.Size() ? _passNames[index] : FString::EMPTY;
}

AMaterial* AMaterial::DefaultMaterial()
{
    // Create on demand
    if (!_defaultMaterial)
    {
        _defaultMaterial = new AMaterial();
        FPass* pass = _defaultMaterial->CreatePass("opaque");
        pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");

        pass = _defaultMaterial->CreatePass("opaqueadd");
        pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");
        pass->SetBlendMode(EBlendMode::ADD);
        pass->_depthWrite = false;

        pass = _defaultMaterial->CreatePass("shadow");
        pass->SetShaders("Shader/Shadow", "Shader/Shadow");
        pass->_colorWriteMask = COLORMASK_NONE;
    }

    return _defaultMaterial.Get();
}


}
