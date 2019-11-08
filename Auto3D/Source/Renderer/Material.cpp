#include "../Debug/Profiler.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ShaderVariation.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Resource/JSONFile.h"
#include "../Resource/ResourceCache.h"

#include "Material.h"

#include "../Engine/ModuleManager.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

SharedPtr<Material> Material::_defaultMaterial;
HashMap<String, unsigned char> Material::_passIndices;
Vector<String> Material::_passNames;
unsigned char Material::_nextPassIndex = 0;

Pass::Pass(Material* parent, const String& name) :
    _parent(parent),
    _name(name),
    _shaderHash(0),
    _shadersLoaded(false)
{
    Reset();
}

Pass::~Pass()
{
}

bool Pass::LoadJSON(const JSONValue& source)
{
    if (source.Contains("vs"))
        _shaderNames[ShaderStage::VS] = source["vs"].GetString();
    if (source.Contains("ps"))
        _shaderNames[ShaderStage::PS] = source["ps"].GetString();
    if (source.Contains("vsDefines"))
        _shaderDefines[ShaderStage::VS] = source["vsDefines"].GetString();
    if (source.Contains("psDefines"))
        _shaderDefines[ShaderStage::PS] = source["psDefines"].GetString();

    if (source.Contains("depthFunc"))
        _depthFunc = (CompareFunc::Type)String::ListIndex(source["depthFunc"].GetString(), compareFuncNames, CompareFunc::LESS_EQUAL);
    if (source.Contains("depthWrite"))
        _depthWrite = source["depthWrite"].GetBool();
    if (source.Contains("depthClip"))
        _depthClip = source["depthClip"].GetBool();
    if (source.Contains("alphaToCoverage"))
        _alphaToCoverage = source["alphaToCoverage"].GetBool();
    if (source.Contains("colorWriteMask"))
        _colorWriteMask = (unsigned char)source["colorWriteMask"].GetNumber();
    if (source.Contains("blendMode"))
        _blendMode = blendModes[String::ListIndex(source["blendMode"].GetString(), blendModeNames, BlendMode::REPLACE)];
    else
    {
        if (source.Contains("blendEnable"))
            _blendMode._blendEnable = source["blendEnable"].GetBool();
        if (source.Contains("srcBlend"))
            _blendMode._srcBlend = (BlendFactor::Type)String::ListIndex(source["srcBlend"].GetString(), blendFactorNames, BlendFactor::ONE);
        if (source.Contains("destBlend"))
            _blendMode._destBlend = (BlendFactor::Type)String::ListIndex(source["destBlend"].GetString(), blendFactorNames, BlendFactor::ONE);
        if (source.Contains("blendOp"))
            _blendMode._blendOp = (BlendOp::Type)String::ListIndex(source["blendOp"].GetString(), blendOpNames, BlendOp::ADD);
        if (source.Contains("srcBlendAlpha"))
            _blendMode._srcBlendAlpha = (BlendFactor::Type)String::ListIndex(source["srcBlendAlpha"].GetString(), blendFactorNames, BlendFactor::ONE);
        if (source.Contains("destBlendAlpha"))
            _blendMode._destBlendAlpha = (BlendFactor::Type)String::ListIndex(source["destBlendAlpha"].GetString(), blendFactorNames, BlendFactor::ONE);
        if (source.Contains("blendOpAlpha"))
            _blendMode._blendOpAlpha = (BlendOp::Type)String::ListIndex(source["blendOpAlpha"].GetString(), blendOpNames, BlendOp::ADD);
    }

    if (source.Contains("fillMode"))
        _fillMode = (FillMode::Type)String::ListIndex(source["fillMode"].GetString(), fillModeNames, FillMode::SOLID);
    if (source.Contains("cullMode"))
        _cullMode = (CullMode::Type)String::ListIndex(source["cullMode"].GetString(), cullModeNames, CullMode::BACK);

    OnShadersChanged();
    return true;
}

bool Pass::SaveJSON(JSONValue& dest)
{
    dest.SetEmptyObject();

    if (_shaderNames[ShaderStage::VS].Length())
        dest["vs"] = _shaderNames[ShaderStage::VS];
    if (_shaderNames[ShaderStage::PS].Length())
        dest["ps"] = _shaderNames[ShaderStage::PS];
    if (_shaderDefines[ShaderStage::VS].Length())
        dest["vsDefines"] = _shaderDefines[ShaderStage::VS];
    if (_shaderDefines[ShaderStage::PS].Length())
        dest["psDefines"] = _shaderDefines[ShaderStage::PS];

    dest["depthFunc"] = compareFuncNames[_depthFunc];
    dest["depthWrite"] = _depthWrite;
    dest["depthClip"] = _depthClip;
    dest["alphaToCoverage"] = _alphaToCoverage;
    dest["colorWriteMask"] = _colorWriteMask;

    // Prefer saving a predefined blend mode if possible for better readability
    bool blendModeFound = false;
    for (size_t i = 0; i < BlendMode::Count; ++i)
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

void Pass::SetBlendMode(BlendMode::Type mode)
{
    _blendMode = blendModes[mode];
}

void Pass::SetShaders(const String& vsName, const String& psName, const String& vsDefines, const String& psDefines)
{
    _shaderNames[ShaderStage::VS] = vsName;
    _shaderNames[ShaderStage::PS] = psName;
    _shaderDefines[ShaderStage::VS] = vsDefines;
    _shaderDefines[ShaderStage::PS] = psDefines;
    OnShadersChanged();
}

void Pass::Reset()
{
    _depthFunc = CompareFunc::LESS_EQUAL;
    _depthWrite = true;
    _depthClip = true;
    _alphaToCoverage = false;
    _colorWriteMask = COLORMASK_ALL;
    _blendMode.Reset();
    _cullMode = CullMode::BACK;
    _fillMode = FillMode::SOLID;
}

Material* Pass::Parent() const
{
    return _parent;
}

void Pass::OnShadersChanged()
{
    // Reset existing variations
    for (size_t i = 0; i < ShaderStage::Count; ++i)
    {
        _shaders[i].Reset();
        _shaderVariations[i].Clear();
    }

    _shadersLoaded = false;

    // Combine and trim the shader defines
    for (size_t i = 0; i < ShaderStage::Count; ++i)
    {
        const String& materialDefines = _parent->ShaderDefines((ShaderStage::Type)i);
        if (materialDefines.Length())
            _combinedShaderDefines[i] = (materialDefines.Trimmed() + " " + _shaderDefines[i]).Trimmed();
        else
            _combinedShaderDefines[i] = _shaderDefines[i].Trimmed();
    }

    _shaderHash = StringHash(_shaderNames[ShaderStage::VS] + _shaderNames[ShaderStage::PS] + _combinedShaderDefines[ShaderStage::VS] +
        _combinedShaderDefines[ShaderStage::PS]).Value();
}

Material::Material()
{
}

Material::~Material()
{
}

void Material::RegisterObject()
{
    RegisterFactory<Material>();
}

bool Material::BeginLoad(Stream& source)
{
    PROFILE(BeginLoadMaterial);

    _loadJSON = new JSONFile();
    if (!_loadJSON->Load(source))
        return false;

    const JSONValue& root = _loadJSON->Root();

    _shaderDefines[ShaderStage::VS].Clear();
    _shaderDefines[ShaderStage::PS].Clear();
    if (root.Contains("vsDefines"))
        _shaderDefines[ShaderStage::VS] = root["vsDefines"].GetString();
    if (root.Contains("psDefines"))
        _shaderDefines[ShaderStage::PS] = root["psDefines"].GetString();

    return true;
}

bool Material::EndLoad()
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

	ResourceCache* cache = ModuleManager::Get().CacheModule();

    if (root.Contains("textures"))
    {
        const JSONObject& jsonTextures = root["textures"].GetObject();
        for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
            SetTexture(it->_first.ToInt(), cache->LoadResource<Texture>(it->_second.GetString()));
    }
	if (root.Contains("texturesMap"))
	{
		const JSONObject& jsonTextures = root["texturesMap"].GetObject();
		Vector<Image*> imageData;
		Vector<ImageLevel> faces;

		for (auto it = jsonTextures.Begin(); it != jsonTextures.End(); ++it)
			imageData.Push(cache->LoadResource<Image>(it->_second.GetString()));

		
		for (int i = 0; i < MAX_CUBE_FACES; ++i)
		{
			faces.Push(imageData[i]->GetLevel(0));
		}

		SharedPtr<Texture>textureCube(new Texture());
		textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, imageData[0]->GetLevel(0)._size, imageData[0]->GetFormat(), 1, &faces[0]);
		textureCube->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
		textureCube->SetDataLost(false);
		SetTexture(jsonTextures.Begin()->_first.ToInt(), textureCube);
	}
    _loadJSON.Reset();
    return true;
}

bool Material::Save(Stream& dest)
{
    PROFILE(SaveMaterial);

    JSONFile saveJSON;
    JSONValue& root = saveJSON.Root();
    root.SetEmptyObject();

    if (_shaderDefines[ShaderStage::VS].Length())
        root["vsDefines"] = _shaderDefines[ShaderStage::VS];
    if (_shaderDefines[ShaderStage::PS].Length())
        root["psDefines"] = _shaderDefines[ShaderStage::PS];
    
    if (_passes.Size())
    {
        root["passes"].SetEmptyObject();
        for (auto it = _passes.Begin(); it != _passes.End(); ++it)
        {
            Pass* pass = *it;
            if (pass)
                pass->SaveJSON(root["passes"][pass->GetName()]);
        }
    }

    if (_constantBuffers[ShaderStage::VS])
        _constantBuffers[ShaderStage::VS]->SaveJSON(root["vsConstantBuffer"]);
    if (_constantBuffers[ShaderStage::PS])
        _constantBuffers[ShaderStage::PS]->SaveJSON(root["psConstantBuffer"]);

    root["textures"].SetEmptyObject();
    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
    {
        if (_textures[i])
            root["textures"][String((int)i)] = _textures[i]->Name();
    }

    return saveJSON.Save(dest);
}

Pass* Material::CreatePass(const String& name)
{
    size_t index = PassIndex(name);
    if (_passes.Size() <= index)
        _passes.Resize(index + 1);

    if (!_passes[index])
        _passes[index] = new Pass(this, name);
    
    return _passes[index];
}

void Material::RemovePass(const String& name)
{
    size_t index = PassIndex(name, false);
    if (index < _passes.Size())
        _passes[index].Reset();
}

void Material::SetTexture(size_t index, Texture* texture)
{
    if (index < MAX_MATERIAL_TEXTURE_UNITS)
        _textures[index] = texture;
}

void Material::ResetTextures()
{
    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
        _textures[i].Reset();
}

void Material::SetConstantBuffer(ShaderStage::Type stage, ConstantBuffer* buffer)
{
    if (stage < ShaderStage::Count)
        _constantBuffers[stage] = buffer;
}

void Material::SetShaderDefines(const String& vsDefines, const String& psDefines)
{
    _shaderDefines[ShaderStage::VS] = vsDefines;
    _shaderDefines[ShaderStage::PS] = psDefines;
    
    for (auto it = _passes.Begin(); it != _passes.End(); ++it)
    {
        Pass* pass = *it;
        if (pass)
            pass->OnShadersChanged();
    }
}

Pass* Material::FindPass(const String& name) const
{
    return GetPass(PassIndex(name, false));
}

Pass* Material::GetPass(unsigned char index) const
{
    return index < _passes.Size() ? _passes[index].Get() : nullptr;
}

Texture* Material::GetTexture(size_t index) const
{
    return index < MAX_MATERIAL_TEXTURE_UNITS ? _textures[index].Get() : nullptr;
}

ConstantBuffer* Material::GetConstantBuffer(ShaderStage::Type stage) const
{
    return stage < ShaderStage::Count ? _constantBuffers[stage].Get() : nullptr;
}

const String& Material::ShaderDefines(ShaderStage::Type stage) const
{
    return stage < ShaderStage::Count ? _shaderDefines[stage] : String::EMPTY;
}

unsigned char Material::PassIndex(const String& name, bool createNew)
{
    String nameLower = name.ToLower();
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

const String& Material::PassName(unsigned char index)
{
    return index < _passNames.Size() ? _passNames[index] : String::EMPTY;
}

Material* Material::DefaultMaterial()
{
    // Create on demand
    if (!_defaultMaterial)
    {
        _defaultMaterial = new Material();
        Pass* pass = _defaultMaterial->CreatePass("opaque");
        pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");

        pass = _defaultMaterial->CreatePass("opaqueadd");
        pass->SetShaders("Shader/NoTexture", "Shader/NoTexture");
        pass->SetBlendMode(BlendMode::ADD);
        pass->_depthWrite = false;

        pass = _defaultMaterial->CreatePass("shadow");
        pass->SetShaders("Shader/Shadow", "Shader/Shadow");
        pass->_colorWriteMask = COLORMASK_NONE;
    }

    return _defaultMaterial.Get();
}


}
