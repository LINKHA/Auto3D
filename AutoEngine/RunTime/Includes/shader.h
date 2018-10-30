#pragma once
#include "Auto.h"
#include "GLGather.h"
#include "AutoSTL.h"
namespace Auto3D {
struct Vector2;
struct Vector3;
struct Vector4;
struct Color;

class Shader
{
public:
	unsigned int ID;
	Shader() {}
	/**
	* @brief : Constructor generates the shader on the fly
	*/
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	Shader(STRING vertexPath, STRING fragmentPath, STRING geometryPath)
		:Shader(vertexPath.CStr(), fragmentPath.CStr(), geometryPath.CStr())
	{}
	Shader(STRING vertexPath, STRING fragmentPath)
		:Shader(vertexPath.CStr(), fragmentPath.CStr())
	{}
	/**
	* @brief : Activate the shader
	*/
	void Use();
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
	void checkCompileErrors(GLuint shader, std::string type);
};
}
