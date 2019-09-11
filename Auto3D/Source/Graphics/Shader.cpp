#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../Resource/ResourceCache.h"
#include "../IO/File.h"
#include "../IO/FileSystem.h"
#include "Shader.h"
#include "ShaderVariation.h"

namespace Auto3D
{

Shader::Shader() :
    _stage(ShaderStage::VS)
{
}

Shader::~Shader()
{
}

void Shader::RegisterObject()
{
    RegisterFactory<Shader>();
}

bool Shader::BeginLoad(Stream& source)
{
    String extension = Extension(source.Name());
    _stage = (extension == ".vs" || extension == ".vert") ? ShaderStage::VS : ShaderStage::PS;
    _sourceCode.Clear();
    return ProcessIncludes(_sourceCode, source);
}

bool Shader::EndLoad()
{
    // Release existing variations (if any) to allow them to be recompiled with changed code
    for (auto it = _variations.Begin(); it != _variations.End(); ++it)
        it->_second->Release();
    return true;
}

void Shader::Define(ShaderStage::Type stage, const String& code)
{
    _stage = stage;
    _sourceCode = code;
    EndLoad();
}

ShaderVariation* Shader::CreateVariation(const String& definesIn)
{
    StringHash definesHash(definesIn);
    auto it = _variations.Find(definesHash);
    if (it != _variations.End())
        return it->_second.Get();
    
    // If initially not found, normalize the defines and try again
    String defines = NormalizeDefines(definesIn);
    definesHash = defines;
    it = _variations.Find(definesHash);
    if (it != _variations.End())
        return it->_second.Get();

    ShaderVariation* newVariation = new ShaderVariation(this, defines);
    _variations[definesHash] = newVariation;
    return newVariation;
}

bool Shader::ProcessIncludes(String& code, Stream& source)
{
    ResourceCache* cache = Module<ResourceCache>();

    while (!source.IsEof())
    {
        String line = source.ReadLine();

        if (line.StartsWith("#include"))
        {
            String includeFileName = Path(source.Name()) + line.Substring(9).Replaced("\"", "").Trimmed();
            AutoPtr<Stream> includeStream = cache->OpenResource(includeFileName);
            if (!includeStream)
                return false;

            // Add the include file into the current code recursively
            if (!ProcessIncludes(code, *includeStream))
                return false;
        }
        else
        {
            code += line;
            code += "\n";
        }
    }

    // Finally insert an empty line to mark the space between files
    code += "\n";

    return true;
}

String Shader::NormalizeDefines(const String& defines)
{
    String ret;
    Vector<String> definesVec = defines.ToUpper().Split(' ');
    Sort(definesVec.Begin(), definesVec.End());

    for (auto it = definesVec.Begin(); it != definesVec.End(); ++it)
    {
        if (it != definesVec.Begin())
            ret += " ";
        ret += *it;
    }

    return ret;
}

}
