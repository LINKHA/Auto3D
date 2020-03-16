#pragma once
#include "Resource/Resource.h"
#include "Renderer/ShaderProgram.h"

namespace Auto3D
{
#define UNIFORM_SAMPLER "sampler"
#define UNIFORM_END "end"
#define UNIFORM_VEC4 "vec4"
#define UNIFORM_MAT3 "mat3"
#define UNIFORM_MAT4 "mat4"

namespace EMaterialShaderType
{
	enum Data
	{
		DEFAULT,
		SHADOW_HARD,
		SHADOW_PCF,
		SHADOW_ESM,
		SHADOW_VSM,
	};
}

class OJSONFile;
/// Material resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class OMaterial : public OResource
{
	DECLARE_O_CLASS(OMaterial, OResource)

public:
	/// Construct.
	OMaterial();
	/// Destruct.
	virtual ~OMaterial();

	/// Load material from a stream. Return true on success.
	virtual bool BeginLoad(const FString& pathName);
	/// Finalize material loading in the main thread. Return true on success.
	virtual bool EndLoad();

	FShaderProgram& GetShaderProgram();
	FShaderProgram& GetShaderInstanceProgram();

	THashMap<FString,bgfx::UniformHandle>& GetUniforms();

	void SetShaderType(EMaterialShaderType::Data type) { _shaderType = type; }

	EMaterialShaderType::Data GetShaderType() { return _shaderType; }

	static OMaterial* DefaultMaterial();
private:
	/// Default material.
	static OMaterial* _defaultMaterial;
	/// JSON data used for loading.
	UPtr<OJSONFile> _loadJSON;

	FShaderProgram _shaderProgram;
	FShaderProgram _shaderInstanceProgram;
	THashMap<FString, bgfx::UniformHandle> _uniforms;

	EMaterialShaderType::Data _shaderType;
};

}