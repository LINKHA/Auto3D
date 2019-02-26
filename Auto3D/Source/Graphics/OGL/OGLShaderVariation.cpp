#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "../Shader.h"
#include "OGLGraphics.h"
#include "OGLShaderVariation.h"

#include <glad.h>

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

ShaderVariation::ShaderVariation(Shader* parent_, const String& defines) :
    _shader(0),
    _parent(parent_),
    _stage(_parent->Stage()),
    _defines(defines),
    _compiled(false)
{
}

ShaderVariation::~ShaderVariation()
{
    Release();
}

void ShaderVariation::Release()
{
    if (_graphics)
    {
        if (_graphics->GetVertexShader() == this || _graphics->GetPixelShader() == this)
            _graphics->SetShaders(nullptr, nullptr);
        _graphics->CleanupShaderPrograms(this);
    }

    if (_shader)
    {
        glDeleteShader(_shader);
        _shader = 0;
    }

    _compiled = false;
}

bool ShaderVariation::Compile()
{
    if (_compiled)
        return _shader != 0;

    PROFILE(CompileShaderVariation);

    // Do not retry without a Release() inbetween
    _compiled = true;

    if (!_graphics || !_graphics->IsInitialized())
    {
        ErrorString("Can not compile shader without initialized Graphics subsystem");
        return false;
    }
    if (!_parent)
    {
        ErrorString("Can not compile shader without parent shader resource");
        return false;
    }

    _shader = glCreateShader(_stage == SHADER_VS ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    if (!_shader)
    {
        ErrorString("Could not create shader object");
        return false;
    }

    // Collect defines into macros
    Vector<String> defineNames = _defines.Split(' ');

    for (auto it = defineNames.Begin(); it != defineNames.End(); ++it)
        it->Replace('=', ' ');

    const String& originalShaderCode = _parent->SourceCode();
    String shaderCode;

    // Check if the shader code contains a version define
    size_t verStart = originalShaderCode.Find('#');
    size_t verEnd = 0;
    if (verStart != String::NPOS)
    {
        if (originalShaderCode.Substring(verStart + 1, 7) == "version")
        {
            verEnd = verStart + 9;
            while (verEnd < originalShaderCode.Length())
            {
                if (IsDigit(originalShaderCode[verEnd]))
                    ++verEnd;
                else
                    break;
            }
            // If version define found, insert it first
            String versionDefine = originalShaderCode.Substring(verStart, verEnd - verStart);
            shaderCode += versionDefine + "\n";
        }
    }

    // Prepend the defines to the shader code
    for (auto it = defineNames.Begin(); it != defineNames.End(); ++it)
        shaderCode += "#define " + *it + "\n";

    // When version define found, do not insert it a second time
    if (verEnd > 0)
        shaderCode += (originalShaderCode.CString() + verEnd);
    else
        shaderCode += originalShaderCode;

    const char* shaderCStr = shaderCode.CString();
    glShaderSource(_shader, 1, &shaderCStr, 0);
    glCompileShader(_shader);

    int compiled;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        int length, outLength;
        String errorString;

        glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &length);
        errorString.Resize(length);
        glGetShaderInfoLog(_shader, length, &outLength, &errorString[0]);
        glDeleteShader(_shader);
        _shader = 0;

        ErrorStringF("Could not compile shader %s: %s", FullName().CString(), errorString.CString());
        return false;
    }

    LogString("Compiled shader " + FullName());
    return true;
}

Shader* ShaderVariation::Parent() const
{
    return _parent;
}

String ShaderVariation::FullName() const
{
    if (_parent)
        return _defines.IsEmpty() ? _parent->Name() : _parent->Name() + " (" + _defines + ")";
    else
        return String::EMPTY;
}

}