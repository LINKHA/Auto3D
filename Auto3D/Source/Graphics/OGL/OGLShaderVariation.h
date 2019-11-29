#pragma once

#include "Container/String.h"
#include "Graphics/GPUObject.h"
#include "Graphics/GraphicsDefs.h"

namespace Auto3D
{

class Shader;

/// Compiled shader with specific defines.
class AUTO_API ShaderVariation : public FRefCounted, public GPUObject
{
public:
    /// Construct. Set parent shader and defines but do not compile yet.
    ShaderVariation(Shader* parent, const FString& defines);
    /// Destruct.
    ~ShaderVariation();

    /// Release the compiled shader.
    void Release() override;

    /// Compile. Return true on success. No-op that return previous result if compile already attempted.
    bool Compile();
    
    /// Return the parent shader resource.
    Shader* Parent() const;
    /// Return full name combined from parent resource name and compilation defines.
    FString FullName() const;
    /// Return shader stage.
    ShaderStage::Type GetStage() const { return _stage; }
    /// Return whether compile attempted.
    bool IsCompiled() const { return _compiled; }

    /// Return the OpenGL shader identifier. Used internally and should not be called by portable application code.
    unsigned GetGLShader() const { return _shader; }

private:
    /// OpenGL shader object identifier.
    unsigned _shader;
    /// Parent shader resource.
    TWeakPtr<Shader> _parent;
    /// Shader stage.
    ShaderStage::Type _stage;
    /// Compilation defines.
    FString _defines;
    /// Compile attempted flag.
    bool _compiled;
};

}