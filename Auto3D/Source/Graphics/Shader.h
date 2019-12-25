#pragma once

#include "Resource/Resource.h"
#include "GraphicsDefs.h"

namespace Auto3D
{

class FShaderVariation;

/// %AShader resource. Defines either vertex or pixel shader source code, from which variations can be compiled by specifying defines.
class AUTO_API AShader : public AResource
{
    DECLARE_CLASS(AShader,AResource)

public:
    /// Construct.
    AShader();
    /// Destruct.
    ~AShader();

    /// Load shader code from a stream. Return true on success.
    bool BeginLoad(FStream& source) override;
    /// Finish shader loading in the main thread. Return true on success.
    bool EndLoad() override;

    /// Define shader stage and source code. All existing variations are destroyed.
    void Define(EShaderStage::Type stage, const FString& code);
    /// Create and return a variation with defines, eg. "PERPIXEL NORMALMAP NUMLIGHTS=4". Existing variation is returned if possible. Variations should be cached to avoid repeated query.
    FShaderVariation* CreateVariation(const FString& defines = FString::EMPTY);
    
    /// Return shader stage.
    EShaderStage::Type GetStage() const { return _stage; }
    /// Return shader source code.
    const FString& GetSourceCode() const { return _sourceCode; }

    /// Sort the defines and strip extra spaces to prevent creation of unnecessary duplicate shader variations. When requesting variations, the defines should preferably be normalized already to save time.
    static FString NormalizeDefines(const FString& defines);

private:
    /// Process include statements in the shader source code recursively. Return true if successful.
    bool ProcessIncludes(FString& code, FStream& source);

    /// %AShader variations.
    THashMap<FStringHash, TSharedPtr<FShaderVariation> > _variations;
    /// %AShader stage.
    EShaderStage::Type _stage;
    /// %AShader source code.
    FString _sourceCode;
};

}
