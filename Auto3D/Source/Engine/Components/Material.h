#pragma once

#include "Container/AutoPtr.h"
#include "Graphics/GraphicsDefs.h"
#include "Resource/Resource.h"

namespace Auto3D
{

class FConstantBuffer;
class JSONFile;
class FJSONValue;
class AMaterial;
class AShader;
class FShaderVariation;
class ATexture;

/// Render pass, which defines render state and shaders. A material may define several of these.
class FPass : public FRefCounted
{
public:
    /// Construct.
    FPass(AMaterial* parent, const FString& name);
    /// Destruct.
    ~FPass();

    /// Load from JSON data. Return true on success.
    bool LoadJSON(const FJSONValue& source);
    /// Save to JSON data. Return true on success.
    bool SaveJSON(FJSONValue& dest);
    /// Set a predefined blend mode.
    void SetBlendMode(EBlendMode::Type mode);
    /// Set shader names and defines.
    void SetShaders(const FString& vsName, const FString& psName, const FString& vsDefines = FString::EMPTY, const FString& psDefines = FString::EMPTY);
    /// Reset render state to defaults.
    void Reset();

    /// Return parent material resource.
    AMaterial* Parent() const;
    /// Return pass name.
    const FString& GetName() const { return _name; }
    /// Return shader name by stage.
    const FString& GetShaderName(EShaderStage::Type stage) const { return _shaderNames[stage]; }
    /// Return shader defines by stage.
    const FString& GetShaderDefines(EShaderStage::Type stage) const { return _shaderDefines[stage]; }
    /// Return combined shader defines from the material and pass by stage.
    const FString& GetCombinedShaderDefines(EShaderStage::Type stage) const { return _combinedShaderDefines[stage]; }
    /// Return shader hash value for state sorting.
    unsigned GetShaderHash() const { return _shaderHash; }

    /// Refresh the combined shader defines and shader hash and clear any cached shader variations. Called internally.
    void OnShadersChanged();

    /// Depth compare function.
    ECompareFunc::Type _depthFunc;
    /// Depth write enable.
    bool _depthWrite;
    /// Depth clipping enable.
    bool _depthClip;
    /// Alpha to coverage enable.
    bool _alphaToCoverage;
    /// Color write mask.
    unsigned char _colorWriteMask;
    /// Blend mode parameters.
    FBlendModeDesc _blendMode;
    /// Polygon culling mode.
    ECullMode::Type _cullMode;
    /// Polygon fill mode.
    EFillMode::Type _fillMode;
    /// AShader resources. Filled by Renderer.
    TSharedPtr<AShader> _shaders[EShaderStage::Count];
    /// Cached shader variations. Filled by Renderer.
    THashMap<unsigned short, TWeakPtr<FShaderVariation> > _shaderVariations[EShaderStage::Count];
    /// AShader load attempted flag. Filled by Renderer.
    bool _shadersLoaded;

private:
    /// Parent material resource.
    TWeakPtr<AMaterial> _parent;
    /// FPass name.
    FString _name;
    /// AShader names.
    FString _shaderNames[EShaderStage::Count];
    /// AShader defines.
    FString _shaderDefines[EShaderStage::Count];
    /// Combined shader defines from both the pass and material. Filled by Renderer.
    FString _combinedShaderDefines[EShaderStage::Count];
    /// AShader hash calculated from names and defines.
    unsigned _shaderHash;
};

/// %AMaterial resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class AMaterial : public AResource
{
	REGISTER_OBJECT_CLASS(AMaterial, AResource)

public:
    /// Construct.
    AMaterial();
    /// Destruct.
	virtual ~AMaterial();

    /// Register object factory.
    static void RegisterObject();

    /// Load material from a stream. Return true on success.
	virtual bool BeginLoad(FStream& source);
    /// Finalize material loading in the main thread. Return true on success.
	virtual bool EndLoad();
    /// Save the material to a stream. Return true on success.
	virtual bool Save(FStream& dest);

    /// Create and return a new pass. If pass with same name exists, it will be returned.
    FPass* CreatePass(const FString& name);
    /// Remove a pass.
    void RemovePass(const FString& name);
    /// Set a texture.
    void SetTexture(size_t index, ATexture* texture);
    /// Reset all texture assignments.
    void ResetTextures();
    /// Set a constant buffer.
    void SetConstantBuffer(EShaderStage::Type stage, FConstantBuffer* buffer);
    /// Set global shader defines. Clears existing shader cached variations from all passes.
    void SetShaderDefines(const FString& vsDefines = FString::EMPTY, const FString& psDefines = FString::EMPTY);

    /// Return pass by name or null if not found. Should not be called in performance-sensitive rendering loops.
    FPass* FindPass(const FString& name) const;
    /// Return pass by index or null if not found.
    FPass* GetPass(unsigned char index) const;
    /// Return texture by texture unit.
    ATexture* GetTexture(size_t index) const;
    /// Return constant buffer by stage.
    FConstantBuffer* GetConstantBuffer(EShaderStage::Type stage) const;
    /// Return shader defines by stage.
    const FString& ShaderDefines(EShaderStage::Type stage) const;

    /// Return pass index from name. By default reserve a new index if the name was not known.
    static unsigned char PassIndex(const FString& name, bool createNew = true);
    /// Return pass name by index.
    static const FString& PassName(unsigned char index);
    /// Return a default opaque untextured material.
    static AMaterial* DefaultMaterial();

    /// AMaterial textures.
    TSharedPtr<ATexture> _textures[MAX_MATERIAL_TEXTURE_UNITS];
    /// FConstant buffers.
    TSharedPtr<FConstantBuffer> _constantBuffers[EShaderStage::Count];

protected:
    /// Passes by index.
    TVector<TSharedPtr<FPass> > _passes;
    /// Global shader defines.
    FString _shaderDefines[EShaderStage::Count];
    /// JSON data used for loading.
    TUniquePtr<JSONFile> _loadJSON;

    /// Default material.
    static TSharedPtr<AMaterial> _defaultMaterial;
    /// FPass name to index mapping.
    static THashMap<FString, unsigned char> _passIndices;
    /// FPass names by index.
    static TVector<FString> _passNames;
    /// Next free pass index.
    static unsigned char _nextPassIndex;
};

}
