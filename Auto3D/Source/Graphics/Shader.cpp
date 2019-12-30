#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Resource/ResourceCache.h"
#include "IO/File.h"
#include "IO/FileSystem.h"
#include "Core/Modules/ModuleManager.h"

#include "Shader.h"
#include "ShaderVariation.h"

namespace Auto3D
{

REGISTER_CLASS(AShader)
{
	REGISTER_CALSS_FACTORY_IMP(AShader)
	.constructor<>()
	;
}

AShader::AShader() :
    _stage(EShaderStage::VS)
{
}

AShader::~AShader()
{
}

bool AShader::BeginLoad(FStream& source)
{
    FString extension = Extension(source.GetName());
    _stage = (extension == ".vs" || extension == ".vert") ? EShaderStage::VS : EShaderStage::PS;
    _sourceCode.Clear();
    return ProcessIncludes(_sourceCode, source);
}

bool AShader::EndLoad()
{
    // Release existing variations (if any) to allow them to be recompiled with changed code
    for (auto it = _variations.Begin(); it != _variations.End(); ++it)
        it->_second->Release();
    return true;
}

void AShader::Define(EShaderStage::Type stage, const FString& code)
{
    _stage = stage;
    _sourceCode = code;
    EndLoad();
}

FShaderVariation* AShader::CreateVariation(const FString& definesIn)
{
    FStringHash definesHash(definesIn);
    auto it = _variations.Find(definesHash);
    if (it != _variations.End())
        return it->_second.Get();
    
    // If initially not found, normalize the defines and try again
    FString defines = NormalizeDefines(definesIn);
    definesHash = defines;
    it = _variations.Find(definesHash);
    if (it != _variations.End())
        return it->_second.Get();

    FShaderVariation* newVariation = new FShaderVariation(this, defines);
    _variations[definesHash] = newVariation;
    return newVariation;
}

bool AShader::ProcessIncludes(FString& code, FStream& source)
{
	FResourceModule* cache = GModuleManager::Get().CacheModule();

    while (!source.IsEof())
    {
        FString line = source.ReadLine();

        if (line.StartsWith("#include"))
        {
            FString includeFileName = Path(source.GetName()) + line.Substring(9).Replaced("\"", "").Trimmed();
            TAutoPtr<FStream> includeStream = cache->OpenResource(includeFileName);
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

FString AShader::NormalizeDefines(const FString& defines)
{
    FString ret;
    TVector<FString> definesVec = defines.ToUpper().Split(' ');
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
