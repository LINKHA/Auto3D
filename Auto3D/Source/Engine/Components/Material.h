#pragma once

#include "Container/AutoPtr.h"
#include "Graphics/GraphicsDefs.h"
#include "Resource/Resource.h"

namespace Auto3D
{

class ConstantBuffer;
class JSONFile;
class JSONValue;
class Material;
class Shader;
class ShaderVariation;
class Texture;

/// Render pass, which defines render state and shaders. A material may define several of these.
class Pass : public FRefCounted
{
public:
    /// Construct.
    Pass(Material* parent, const FString& name);
    /// Destruct.
    ~Pass();

    /// Load from JSON data. Return true on success.
    bool LoadJSON(const JSONValue& source);
    /// Save to JSON data. Return true on success.
    bool SaveJSON(JSONValue& dest);
    /// Set a predefined blend mode.
    void SetBlendMode(BlendMode::Type mode);
    /// Set shader names and defines.
    void SetShaders(const FString& vsName, const FString& psName, const FString& vsDefines = FString::EMPTY, const FString& psDefines = FString::EMPTY);
    /// Reset render state to defaults.
    void Reset();

    /// Return parent material resource.
    Material* Parent() const;
    /// Return pass name.
    const FString& GetName() const { return _name; }
    /// Return shader name by stage.
    const FString& GetShaderName(ShaderStage::Type stage) const { return _shaderNames[stage]; }
    /// Return shader defines by stage.
    const FString& GetShaderDefines(ShaderStage::Type stage) const { return _shaderDefines[stage]; }
    /// Return combined shader defines from the material and pass by stage.
    const FString& GetCombinedShaderDefines(ShaderStage::Type stage) const { return _combinedShaderDefines[stage]; }
    /// Return shader hash value for state sorting.
    unsigned GetShaderHash() const { return _shaderHash; }

    /// Refresh the combined shader defines and shader hash and clear any cached shader variations. Called internally.
    void OnShadersChanged();

    /// Depth compare function.
    CompareFunc::Type _depthFunc;
    /// Depth write enable.
    bool _depthWrite;
    /// Depth clipping enable.
    bool _depthClip;
    /// Alpha to coverage enable.
    bool _alphaToCoverage;
    /// Color write mask.
    unsigned char _colorWriteMask;
    /// Blend mode parameters.
    BlendModeDesc _blendMode;
    /// Polygon culling mode.
    CullMode::Type _cullMode;
    /// Polygon fill mode.
    FillMode::Type _fillMode;
    /// Shader resources. Filled by Renderer.
    TSharedPtr<Shader> _shaders[ShaderStage::Count];
    /// Cached shader variations. Filled by Renderer.
    THashMap<unsigned short, TWeakPtr<ShaderVariation> > _shaderVariations[ShaderStage::Count];
    /// Shader load attempted flag. Filled by Renderer.
    bool _shadersLoaded;

private:
    /// Parent material resource.
    TWeakPtr<Material> _parent;
    /// Pass name.
    FString _name;
    /// Shader names.
    FString _shaderNames[ShaderStage::Count];
    /// Shader defines.
    FString _shaderDefines[ShaderStage::Count];
    /// Combined shader defines from both the pass and material. Filled by Renderer.
    FString _combinedShaderDefines[ShaderStage::Count];
    /// Shader hash calculated from names and defines.
    unsigned _shaderHash;
};

/// %Material resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class Material : public AResource
{
	REGISTER_OBJECT_CLASS(Material, AResource)

public:
    /// Construct.
    Material();
    /// Destruct.
	virtual ~Material();

    /// Register object factory.
    static void RegisterObject();

    /// Load material from a stream. Return true on success.
	virtual bool BeginLoad(Stream& source);
    /// Finalize material loading in the main thread. Return true on success.
	virtual bool EndLoad();
    /// Save the material to a stream. Return true on success.
	virtual bool Save(Stream& dest);

    /// Create and return a new pass. If pass with same name exists, it will be returned.
    Pass* CreatePass(const FString& name);
    /// Remove a pass.
    void RemovePass(const FString& name);
    /// Set a texture.
    void SetTexture(size_t index, Texture* texture);
    /// Reset all texture assignments.
    void ResetTextures();
    /// Set a constant buffer.
    void SetConstantBuffer(ShaderStage::Type stage, ConstantBuffer* buffer);
    /// Set global shader defines. Clears existing shader cached variations from all passes.
    void SetShaderDefines(const FString& vsDefines = FString::EMPTY, const FString& psDefines = FString::EMPTY);

    /// Return pass by name or null if not found. Should not be called in performance-sensitive rendering loops.
    Pass* FindPass(const FString& name) const;
    /// Return pass by index or null if not found.
    Pass* GetPass(unsigned char index) const;
    /// Return texture by texture unit.
    Texture* GetTexture(size_t index) const;
    /// Return constant buffer by stage.
    ConstantBuffer* GetConstantBuffer(ShaderStage::Type stage) const;
    /// Return shader defines by stage.
    const FString& ShaderDefines(ShaderStage::Type stage) const;

    /// Return pass index from name. By default reserve a new index if the name was not known.
    static unsigned char PassIndex(const FString& name, bool createNew = true);
    /// Return pass name by index.
    static const FString& PassName(unsigned char index);
    /// Return a default opaque untextured material.
    static Material* DefaultMaterial();

    /// Material textures.
    TSharedPtr<Texture> _textures[MAX_MATERIAL_TEXTURE_UNITS];
    /// Constant buffers.
    TSharedPtr<ConstantBuffer> _constantBuffers[ShaderStage::Count];

protected:
    /// Passes by index.
    TVector<TSharedPtr<Pass> > _passes;
    /// Global shader defines.
    FString _shaderDefines[ShaderStage::Count];
    /// JSON data used for loading.
    TUniquePtr<JSONFile> _loadJSON;

    /// Default material.
    static TSharedPtr<Material> _defaultMaterial;
    /// Pass name to index mapping.
    static THashMap<FString, unsigned char> _passIndices;
    /// Pass names by index.
    static TVector<FString> _passNames;
    /// Next free pass index.
    static unsigned char _nextPassIndex;
};

}
