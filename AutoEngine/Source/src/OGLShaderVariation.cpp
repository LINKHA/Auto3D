#include "ShaderVariation.h"
#if AUTO_OPENGL

namespace Auto3D {


bool ShaderVariation::Create()
{
	unsigned int vertex, fragment;

	const char* vsCode = _vsShaderCode.CStr();
	const char* fsCode = _fsShaderCode.CStr();
	const char* gsCode = _gsShaderCode.CStr();

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vsCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fsCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	unsigned int geometry;
	if (!_gsShaderCode.Empty())
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gsCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	// shader Program
	_object.name = glCreateProgram();
	glAttachShader(_object.name, vertex);
	glAttachShader(_object.name, fragment);
	if (!_gsShaderCode.Empty())
		glAttachShader(_object.name, geometry);

	glLinkProgram(_object.name);
	checkCompileErrors(_object.name, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!_gsShaderCode.Empty())
		glDeleteShader(geometry);
	return true;
}

void ShaderVariation::Use()
{
	glUseProgram(_object.name);
}

void ShaderVariation::SetBool(const STRING &name, bool value) const
{
	glUniform1i(glGetUniformLocation(_object.name, name.CStr()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetInt(const STRING &name, int value) const
{
	glUniform1i(glGetUniformLocation(_object.name, name.CStr()), value);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetFloat(const STRING &name, float value) const
{
	glUniform1f(glGetUniformLocation(_object.name, name.CStr()), value);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec2(const STRING &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(_object.name, name.CStr()), 1, &value[0]);
}
void ShaderVariation::SetVec2(const STRING &name, const Vector2 &value) const
{
	glUniform2f(glGetUniformLocation(_object.name, name.CStr()), value.x, value.y);
}
void ShaderVariation::SetVec2(const STRING &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(_object.name, name.CStr()), x, y);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec3(const STRING &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(_object.name, name.CStr()), 1, &value[0]);
}
void ShaderVariation::SetVec3(const STRING &name, const Color &value) const
{
	glUniform3f(glGetUniformLocation(_object.name, name.CStr()), value.r, value.g, value.b);
}
void ShaderVariation::SetVec3(const STRING &name, const Vector3 &value) const
{
	glUniform3f(glGetUniformLocation(_object.name, name.CStr()), value.x, value.y, value.z);
}
void ShaderVariation::SetVec3(const STRING &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(_object.name, name.CStr()), x, y, z);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec4(const STRING &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(_object.name, name.CStr()), 1, &value[0]);
}
void ShaderVariation::SetVec4(const STRING &name, const Vector4 &value) const
{
	glUniform4f(glGetUniformLocation(_object.name, name.CStr()), value.x, value.y, value.z, value.w);
}
void ShaderVariation::SetVec4(const STRING &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(_object.name, name.CStr()), x, y, z, w);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat2(const STRING &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(_object.name, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat3(const STRING &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(_object.name, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat4(const STRING &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_object.name, name.CStr()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderVariation::checkCompileErrors(GLuint shader, STRING type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			AutoCout << "SHADER_COMPILATION_ERROR of type: " << type << "\t" << infoLog << AutoEndl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			AutoCout << "PROGRAM_LINKING_ERROR of type: " << type << "\t" << infoLog << AutoEndl;
		}
	}
}



}

#endif