#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Graphics/Shader.h"
#include "OGLGraphics.h"
#include "OGLShaderProgram.h"
#include "OGLShaderVariation.h"
#include "OGLVertexBuffer.h"

#include <glad.h>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const size_t MAX_NAME_LENGTH = 256;

int NumberPostfix(const String& str)
{
    for (size_t i = 0; i < str.Length(); ++i)
    {
        if (IsDigit(str[i]))
            return String::ToInt(str.CString() + i);
    }

    return -1;
}

ShaderProgram::ShaderProgram(ShaderVariation* vs, ShaderVariation* ps) :
    _program(0),
    _vs(vs),
    _ps(ps)
{
}

ShaderProgram::~ShaderProgram()
{
    Release();
}

void ShaderProgram::Release()
{
    if (_program)
    {
        glDeleteProgram(_program);
        _program = 0;
    }
}

bool ShaderProgram::Link()
{
    PROFILE(LinkShaderProgram);

    Release();

    if (!_graphics || !_graphics->IsInitialized())
    {
        ErrorString("Can not link shader program without initialized Graphics subsystem");
        return false;
    }
    if (!_vs || !_ps)
    {
        ErrorString("Shader(s) are null, can not link shader program");
        return false;
    }
    if (!_vs->GetGLShader() || !_ps->GetGLShader())
    {
        ErrorString("Shaders have not been compiled, can not link shader program");
        return false;
    }
    
    const String& vsSourceCode = _vs->Parent() ? _vs->Parent()->GetSourceCode() : String::EMPTY;
    const String& psSourceCode = _ps->Parent() ? _ps->Parent()->GetSourceCode() : String::EMPTY;

    _program = glCreateProgram();
    if (!_program)
    {
        ErrorString("Could not create shader program");
        return false;
    }

    glAttachShader(_program, _vs->GetGLShader());
    glAttachShader(_program, _ps->GetGLShader());
    glLinkProgram(_program);

    int linked;
    glGetProgramiv(_program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        int length, outLength;
        String errorString;

        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
        errorString.Resize(length);
        glGetProgramInfoLog(_program, length, &outLength, &errorString[0]);
        glDeleteProgram(_program);
        _program = 0;

        ErrorStringF("Could not link shaders %s: %s", FullName().CString(), errorString.CString());
        return false;
    }

    LogStringF("Linked shaders %s", FullName().CString());

    glUseProgram(_program);

    char nameBuffer[MAX_NAME_LENGTH];
    int numAttributes, numUniforms, numUniformBlocks, nameLength, numElements;
    GLenum type;

    _attributes.Clear();

    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    for (int i = 0; i < numAttributes; ++i)
    {
        glGetActiveAttrib(_program, i, (GLsizei)MAX_NAME_LENGTH, &nameLength, &numElements, &type, nameBuffer);
        
        VertexAttribute newAttribute;
        newAttribute._name = String(nameBuffer, nameLength);
        newAttribute._semantic = ElementSemantic::POSITION;
        newAttribute._index = 0;

        for (size_t j = 0; elementSemanticNames[j]; ++j)
        {
            if (newAttribute._name.StartsWith(elementSemanticNames[j], false))
            {
                int index = NumberPostfix(newAttribute._name);
                if (index >= 0)
                    newAttribute._index = (unsigned char)index;
                break;
            }
            newAttribute._semantic = (ElementSemantic::Type)(newAttribute._semantic + 1);
        }

        if (newAttribute._semantic == ElementSemantic::Count)
        {
            WarningStringF("Found vertex attribute %s with no known semantic in shader program %s", newAttribute._name.CString(), FullName().CString());
            continue;
        }

        newAttribute._location = glGetAttribLocation(_program, newAttribute._name.CString());
        _attributes.Push(newAttribute);
    }

    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
    int numTextures = 0;
    for (int i = 0; i < numUniforms; ++i)
    {
        glGetActiveUniform(_program, i, MAX_NAME_LENGTH, &nameLength, &numElements, &type, nameBuffer);

        String name(nameBuffer, nameLength);
#ifndef AUTO_OPENGL_ES
        if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_SHADOW)
#else
		if (type >= GL_SAMPLER_2D && type <= GL_SAMPLER_CUBE)
#endif
        {
            // Assign sampler uniforms to a texture unit according to the number appended to the sampler name
            int location = glGetUniformLocation(_program, name.CString());
            int unit = NumberPostfix(name);
            // If no unit number specified, assign in appearance order starting from unit 0
            if (unit < 0)
                unit = numTextures;
            
            // Array samplers may have multiple elements, assign each sequentially
            if (numElements > 1)
            {
                Vector<int> units;
                for (int j = 0; j < numElements; ++j)
                    units.Push(unit++);
                glUniform1iv(location, numElements, &units[0]);
            }
            else
                glUniform1iv(location, 1, &unit);

            numTextures += numElements;
        }
    }

    glGetProgramiv(_program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
    for (int i = 0; i < numUniformBlocks; ++i)
    {
        glGetActiveUniformBlockName(_program, i, (GLsizei)MAX_NAME_LENGTH, &nameLength, nameBuffer);
        
        // Determine whether uniform block belongs to vertex or pixel shader
        String name(nameBuffer, nameLength);
        bool foundVs = vsSourceCode.Contains(name);
        bool foundPs = psSourceCode.Contains(name);
        if (foundVs && foundPs)
        {
            WarningStringF("Found uniform block %s in both vertex and pixel shader in shader program %s");
            continue;
        }

        // Vertex shader constant buffer bindings occupy slots starting from zero to maximum supported, pixel shader bindings
        // from that point onward
        unsigned blockIndex = glGetUniformBlockIndex(_program, name.CString());

        int bindingIndex = NumberPostfix(name);
        // If no number postfix in the name, use the block index
        if (bindingIndex < 0)
            bindingIndex = blockIndex;
        if (foundPs)
            bindingIndex += (unsigned)_graphics->NumVSConstantBuffers();

        glUniformBlockBinding(_program, blockIndex, bindingIndex);
    }

    return true;
}

ShaderVariation* ShaderProgram::VertexShader() const
{
    return _vs;
}

ShaderVariation* ShaderProgram::PixelShader() const
{
    return _ps;
}

String ShaderProgram::FullName() const
{
    return (_vs && _ps) ? _vs->FullName() + " " + _ps->FullName() : String::EMPTY;
}

void ShaderProgram::SetBool(const String& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_program, name.CString()), (int)value);
}

void ShaderProgram::SetInt(const String& name, int value) const
{
	glUniform1i(glGetUniformLocation(_program, name.CString()), value);
}

void ShaderProgram::SetFloat(const String& name, float value) const
{
	glUniform1f(glGetUniformLocation(_program, name.CString()), value);
}

void ShaderProgram::SetVec2(const String& name, const Vector2F& value) const
{
	glUniform2fv(glGetUniformLocation(_program, name.CString()), 1, value.Data());
}

void ShaderProgram::SetVec2(const String& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(_program, name.CString()), x, y);
}

void ShaderProgram::SetVec2s(const String& name, int size, float* pVec2) const
{
	glUniform2fv(glGetUniformLocation(_program, name.CString()),size , pVec2);
}

void ShaderProgram::SetVec3(const String& name, const Vector3F& value) const
{
	glUniform3fv(glGetUniformLocation(_program, name.CString()), 1, value.Data());
}

void ShaderProgram::SetVec3(const String& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(_program, name.CString()), x, y, z);
}

void ShaderProgram::SetVec3s(const String& name, int size, float* pVec3) const
{
	glUniform3fv(glGetUniformLocation(_program, name.CString()), size, pVec3);
}

void ShaderProgram::SetVec4(const String& name, const Vector4F& value) const
{
	glUniform4fv(glGetUniformLocation(_program, name.CString()), 1, value.Data());
}

void ShaderProgram::SetVec4(const String& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(_program, name.CString()), x, y, z, w);
}

void ShaderProgram::SetVec4s(const String& name, int size, float* pVec4)
{
	glUniform4fv(glGetUniformLocation(_program, name.CString()), size, pVec4);
}

void ShaderProgram::SetMat2(const String& name, const Matrix2x2F& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(_program, name.CString()), 1, GL_FALSE, mat.Data());
}

void ShaderProgram::SetMat3(const String& name, const Matrix3x3F& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(_program, name.CString()), 1, GL_FALSE, mat.Data());
}

void ShaderProgram::SetMat4(const String& name, const Matrix4x4F& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_program, name.CString()), 1, GL_FALSE, mat.Data());
}

unsigned ShaderProgram::GetUniformLocation(const String& uniformName) const
{
	return glGetUniformLocation(_program, uniformName.CString());
}

unsigned ShaderProgram::GetAttribLocation(const String& AttribName) const
{
	return glGetAttribLocation(_program, AttribName.CString());
}

}