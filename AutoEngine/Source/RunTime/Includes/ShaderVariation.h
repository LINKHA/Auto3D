#pragma once
#include "Auto.h"
#include "GPUObject.h"
#include "Math/Math.h"
#include "Shader.h"
#include "GraphicsDef.h"
namespace Auto3D {

class ShaderVariation : public GPUObject
{
public:
	ShaderVariation(Shader* shader,ShaderType type = ShaderType::VS);

	~ShaderVariation();
	/**
	* @brief : Release the shader
	*/
	void Release();

	/**
	* @brief : Compile the shader. Return true if successful
	*/
	bool Create();

	void Use();

	void SetGeometryShader(Shader* shader) { _gsShader = shader; }
	/**
	* @brief : Utility uniform functions
	*/
	void SetBool(const STRING &name, bool value) const;
	void SetInt(const STRING &name, int value) const;
	void SetFloat(const STRING &name, float value) const;
	void SetVec2(const STRING &name, const glm::vec2 &value) const;
	void SetVec2(const STRING &name, const Vector2 &value) const;
	void SetVec2(const STRING &name, float x, float y) const;
	void SetVec3(const STRING &name, const glm::vec3 &value) const;
	void SetVec3(const STRING &name, const Color &value) const;
	void SetVec3(const STRING &name, const Vector3 &value) const;
	void SetVec3(const STRING &name, float x, float y, float z) const;
	void SetVec4(const STRING &name, const glm::vec4 &value) const;
	void SetVec4(const STRING &name, const Vector4 &value) const;
	void SetVec4(const STRING &name, float x, float y, float z, float w);
	void SetMat2(const STRING &name, const glm::mat2 &mat) const;
	void SetMat3(const STRING &name, const glm::mat3 &mat) const;
	void SetMat4(const STRING &name, const glm::mat4 &mat) const;
private:
	/**
	* @brief : utility function for checking shader compilation/linking errors.
	*/
	void checkCompileErrors(GLuint shader, STRING type);
private:
	/// VS and FS shader
	SharedPtr<Shader> _shader;
	/// GS shader
	Shader* _gsShader;

	/// shader type
	ShaderType _type;
	/// shader name
	STRING _name;

	unsigned char* data;
};

}