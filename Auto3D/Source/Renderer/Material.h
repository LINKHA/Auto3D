#pragma once

#include "../Base/AutoPtr.h"
#include "../Graphics/GraphicsDefs.h"
#include "../Resource/Resource.h"

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
class Pass : public RefCounted
{
public:
    /// Construct.
    Pass(Material* parent, const String& name);
    /// Destruct.
    ~Pass();

    /// Load from JSON data. Return true on success.
    bool LoadJSON(const JSONValue& source);
    /// Save to JSON data. Return true on success.
    bool SaveJSON(JSONValue& dest);
    /// Set a predefined blend mode.
    void SetBlendMode(BlendMode::Type mode);
    /// Set shader names and defines.
    void SetShaders(const String& vsName, const String& psName, const String& vsDefines = String::EMPTY, const String& psDefines = String::EMPTY);
    /// Reset render state to defaults.
    void Reset();

    /// Return parent material resource.
    Material* Parent() const;
    /// Return pass name.
    const String& GetName() const { return _name; }
    /// Return shader name by stage.
    const String& GetShaderName(ShaderStage::Type stage) const { return _shaderNames[stage]; }
    /// Return shader defines by stage.
    const String& GetShaderDefines(ShaderStage::Type stage) const { return _shaderDefines[stage]; }
    /// Return combined shader defines from the material and pass by stage.
    const String& GetCombinedShaderDefines(ShaderStage::Type stage) const { return _combinedShaderDefines[stage]; }
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
    SharedPtr<Shader> _shaders[ShaderStage::Count];
    /// Cached shader variations. Filled by Renderer.
    HashMap<unsigned short, WeakPtr<ShaderVariation> > _shaderVariations[ShaderStage::Count];
    /// Shader load attempted flag. Filled by Renderer.
    bool _shadersLoaded;

private:
    /// Parent material resource.
    WeakPtr<Material> _parent;
    /// Pass name.
    String _name;
    /// Shader names.
    String _shaderNames[ShaderStage::Count];
    /// Shader defines.
    String _shaderDefines[ShaderStage::Count];
    /// Combined shader defines from both the pass and material. Filled by Renderer.
    String _combinedShaderDefines[ShaderStage::Count];
    /// Shader hash calculated from names and defines.
    unsigned _shaderHash;
};

/// %Material resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class Material : public Resource
{
	REGISTER_OBJECT_CLASS(Material, Resource)

public:
    /// Construct.
    Material();
    /// Destruct.
    ~Material();

    /// Register object factory.
    static void RegisterObject();

    /// Load material from a stream. Return true on success.
	virtual bool BeginLoad(Stream& source);
    /// Finalize material loading in the main thread. Return true on success.
	virtual bool EndLoad();
    /// Save the material to a stream. Return true on success.
	virtual bool Save(Stream& dest);

    /// Create and return a new pass. If pass with same name exists, it will be returned.
    Pass* CreatePass(const String& name);
    /// Remove a pass.
    void RemovePass(const String& name);
    /// Set a texture.
    void SetTexture(size_t index, Texture* texture);
    /// Reset all texture assignments.
    void ResetTextures();
    /// Set a constant buffer.
    void SetConstantBuffer(ShaderStage::Type stage, ConstantBuffer* buffer);
    /// Set global shader defines. Clears existing shader cached variations from all passes.
    void SetShaderDefines(const String& vsDefines = String::EMPTY, const String& psDefines = String::EMPTY);

    /// Return pass by name or null if not found. Should not be called in performance-sensitive rendering loops.
    Pass* FindPass(const String& name) const;
    /// Return pass by index or null if not found.
    Pass* GetPass(unsigned char index) const;
    /// Return texture by texture unit.
    Texture* GetTexture(size_t index) const;
    /// Return constant buffer by stage.
    ConstantBuffer* GetConstantBuffer(ShaderStage::Type stage) const;
    /// Return shader defines by stage.
    const String& ShaderDefines(ShaderStage::Type stage) const;

    /// Return pass index from name. By default reserve a new index if the name was not known.
    static unsigned char PassIndex(const String& name, bool createNew = true);
    /// Return pass name by index.
    static const String& PassName(unsigned char index);
    /// Return a default opaque untextured material.
    static Material* DefaultMaterial();

    /// Material textures.
    SharedPtr<Texture> _textures[MAX_MATERIAL_TEXTURE_UNITS];
    /// Constant buffers.
    SharedPtr<ConstantBuffer> _constantBuffers[ShaderStage::Count];

protected:
    /// Passes by index.
    Vector<SharedPtr<Pass> > _passes;
    /// Global shader defines.
    String _shaderDefines[ShaderStage::Count];
    /// JSON data used for loading.
    AutoPtr<JSONFile> _loadJSON;

    /// Default material.
    static SharedPtr<Material> _defaultMaterial;
    /// Pass name to index mapping.
    static HashMap<String, unsigned char> _passIndices;
    /// Pass names by index.
    static Vector<String> _passNames;
    /// Next free pass index.
    static unsigned char _nextPassIndex;
};

}
