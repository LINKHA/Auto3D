#pragma once
#include "Auto.h"
#include "GraphicsDef.h"
#include "Resource.h"
#include "ConstantBuffer.h"

namespace Auto3D {

class Shader;
class ShaderVariation;
class Texture;
class Material;

/// Render pass, which defines render state and shaders. A material may define several of these.
class Pass 
{
public:
	/// Construct.
	Pass(SharedPtr<Material> parent, const STRING& name);
	/// Destruct.
	~Pass();
	/// Set a predefined blend mode.
	void SetBlendMode(BlendMode mode);
	/// Set shader names and defines.
	void SetShaders(const STRING& vsName, const STRING& psName, const STRING& vsDefines = STRING::EMPTY, const STRING& psDefines = STRING::EMPTY);
	/// Reset render state to defaults.
	void Reset();

	/// Return parent material resource.
	SharedPtr<Material> Parent() const;
	/// Return pass name.
	const STRING& Name() const { return name; }
	/// Return shader name by stage.
	const STRING& ShaderName(ShaderStage stage) const { return shaderNames[(int)stage]; }
	/// Return shader defines by stage.
	const STRING& ShaderDefines(ShaderStage stage) const { return shaderDefines[(int)stage]; }
	/// Return combined shader defines from the material and pass by stage.
	const STRING& CombinedShaderDefines(ShaderStage stage) const { return combinedShaderDefines[(int)stage]; }
	/// Return shader hash value for state sorting.
	unsigned ShaderHash() const { return shaderHash; }

	/// Refresh the combined shader defines and shader hash and clear any cached shader variations. Called internally.
	void OnShadersChanged();

	/// Depth compare function.
	CompareFunc depthFunc;
	/// Depth write enable.
	bool depthWrite;
	/// Depth clipping enable.
	bool depthClip;
	/// Alpha to coverage enable.
	bool alphaToCoverage;
	/// Color write mask.
	unsigned char colorWriteMask;
	/// Blend mode parameters.
	BlendModeDesc blendMode;
	/// Polygon culling mode.
	CullMode cullMode;
	/// Polygon fill mode.
	FillMode fillMode;
	/// Shader resources. Filled by Renderer.
	SharedPtr<Shader> shaders[(int)ShaderStage::Count];
	/// Cached shader variations. Filled by Renderer.
	HASH_MAP<unsigned short, WeakPtr<ShaderVariation> > shaderVariations[(int)ShaderStage::Count];
	/// Shader load attempted flag. Filled by Renderer.
	bool shadersLoaded;

private:
	/// Parent material resource.
	WeakPtr<Material> parent;
	/// Pass name.
	STRING name;
	/// Shader names.
	STRING shaderNames[(int)ShaderStage::Count];
	/// Shader defines.
	STRING shaderDefines[(int)ShaderStage::Count];
	/// Combined shader defines from both the pass and material. Filled by Renderer.
	STRING combinedShaderDefines[(int)ShaderStage::Count];
	/// Shader hash calculated from names and defines.
	unsigned shaderHash;
};


/// %Material resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class Material : public Resource
{
	REGISTER_OBJECT_CLASS(Material, Resource)
public:
	/// Construct.
	explicit Material(SharedPtr<Ambient> ambient);


private:

	/// Default material.
	static SharedPtr<Material> defaultMaterial;
	/// Pass name to index mapping.
	static HASH_MAP<STRING, unsigned char> passIndices;
	/// Pass names by index.
	static VECTOR<STRING> passNames;
	/// Next free pass index.
	static unsigned char nextPassIndex;
};




}