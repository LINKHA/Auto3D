#include "ShaderSet.h"
#include "Math/Math.h"

namespace Auto3D {


void ShaderSet::RegisterVertex(const Shader& shader)
{

}

void ShaderSet::RegisterFragment(const Shader& shader)
{

}

void ShaderSet::RegisterGeometry(const Shader& shader)
{

}

void ShaderSet::SetBool(const STRING &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.CStr()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderSet::SetInt(const STRING &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.CStr()), value);
}
// ------------------------------------------------------------------------
void ShaderSet::SetFloat(const STRING &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.CStr()), value);
}
// ------------------------------------------------------------------------
void ShaderSet::SetVec2(const STRING &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.CStr()), 1, &value[0]);
}
void ShaderSet::SetVec2(const STRING &name, const Vector2 &value) const
{
	glUniform2f(glGetUniformLocation(ID, name.CStr()), value.x, value.y);
}
void ShaderSet::SetVec2(const STRING &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.CStr()), x, y);
}
// ------------------------------------------------------------------------
void ShaderSet::SetVec3(const STRING &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.CStr()), 1, &value[0]);
}
void ShaderSet::SetVec3(const STRING &name, const Color &value) const
{
	glUniform3f(glGetUniformLocation(ID, name.CStr()), value.r, value.g, value.b);
}
void ShaderSet::SetVec3(const STRING &name, const Vector3 &value) const
{
	glUniform3f(glGetUniformLocation(ID, name.CStr()), value.x, value.y, value.z);
}
void ShaderSet::SetVec3(const STRING &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.CStr()), x, y, z);
}
// ------------------------------------------------------------------------
void ShaderSet::SetVec4(const STRING &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.CStr()), 1, &value[0]);
}
void ShaderSet::SetVec4(const STRING &name, const Vector4 &value) const
{
	glUniform4f(glGetUniformLocation(ID, name.CStr()), value.x, value.y, value.z, value.w);
}
void ShaderSet::SetVec4(const STRING &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.CStr()), x, y, z, w);
}
// ------------------------------------------------------------------------
void ShaderSet::SetMat2(const STRING &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderSet::SetMat3(const STRING &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderSet::SetMat4(const STRING &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}



}