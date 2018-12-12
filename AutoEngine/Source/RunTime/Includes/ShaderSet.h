#pragma once
#include "Object.h"
#include "AutoOGL.h"
#include "Ambient.h"

namespace Auto3D {

class Shader;
struct Vector2;
struct Vector3;
struct Vector4;
struct Color;

class ShaderSet : public Object
{
	REGISTER_OBJECT_CLASS(ShaderSet, Object)
public:
	explicit ShaderSet(Ambient* ambient);
	/**
	* @brief : Register vertex
	*/
	void RegisterVertex(const Shader& shader);
	/**
	* @brief : Register fragment
	*/
	void RegisterFragment(const Shader& shader);
	/**
	* @brief : Register geometry
	*/
	void RegisterGeometry(const Shader& shader);
	
	/**
	* @brief : Set bool to shader
	*/
	void SetBool(const STRING &name, bool value) const;
	/**
	* @brief :
	*/
	void SetInt(const STRING &name, int value) const;
	/**
	* @brief :
	*/
	void SetFloat(const STRING &name, float value) const;
	/**
	* @brief :
	*/
	void SetVec2(const STRING &name, const glm::vec2 &value) const;
	/**
	* @brief :
	*/
	void SetVec2(const STRING &name, const Vector2 &value) const;
	/**
	* @brief :
	*/
	void SetVec2(const STRING &name, float x, float y) const;
	/**
	* @brief :
	*/
	void SetVec3(const STRING &name, const glm::vec3 &value) const;
	/**
	* @brief :
	*/
	void SetVec3(const STRING &name, const Color &value) const;
	/**
	* @brief :
	*/
	void SetVec3(const STRING &name, const Vector3 &value) const;
	/**
	* @brief :
	*/
	void SetVec3(const STRING &name, float x, float y, float z) const;
	/**
	* @brief :
	*/
	void SetVec4(const STRING &name, const glm::vec4 &value) const;
	/**
	* @brief :
	*/
	void SetVec4(const STRING &name, const Vector4 &value) const;
	/**
	* @brief :
	*/
	void SetVec4(const STRING &name, float x, float y, float z, float w);
	/**
	* @brief :
	*/
	void SetMat2(const STRING &name, const glm::mat2 &mat) const;
	/**
	* @brief :
	*/
	void SetMat3(const STRING &name, const glm::mat3 &mat) const;
	/**
	* @brief :
	*/
	void SetMat4(const STRING &name, const glm::mat4 &mat) const;
private:

public:
	unsigned int ID;
};

}