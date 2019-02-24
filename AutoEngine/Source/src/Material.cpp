#include "Material.h"

namespace Auto3D {


SharedPtr<Material> Material::defaultMaterial;
HASH_MAP<STRING, unsigned char> Material::passIndices;
VECTOR<STRING> Material::passNames;
unsigned char Material::nextPassIndex = 0;

Pass::Pass(SharedPtr<Material> parent_, const STRING& name_) :
	parent(parent_),
	name(name_),
	shaderHash(0),
	shadersLoaded(false)
{
	Reset();
}

Pass::~Pass()
{
}

void Pass::SetBlendMode(BlendMode mode)
{
	blendMode = blendModes[(int)mode];
}

void Pass::SetShaders(const STRING& vsName, const STRING& psName, const STRING& vsDefines, const STRING& psDefines)
{
	shaderNames[(int)ShaderStage::VS] = vsName;
	shaderNames[(int)ShaderStage::PS] = psName;
	shaderDefines[(int)ShaderStage::VS] = vsDefines;
	shaderDefines[(int)ShaderStage::PS] = psDefines;
	OnShadersChanged();
}

void Pass::Reset()
{
	depthFunc = CompareFunc::LessEqual;
	depthWrite = true;
	depthClip = true;
	alphaToCoverage = false;
	colorWriteMask = COLORMASK_ALL;
	blendMode.Reset();
	cullMode = CullMode::CW;
	fillMode = FillMode::Solid;
}

SharedPtr<Material> Pass::Parent() const
{
	return parent.lock();
}

void Pass::OnShadersChanged()
{
}

Material::Material(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}

Material::~Material()
{
}

}