#pragma once

#include "../../Base/String.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Auto3D
{

class Graphics;
class ShaderVariation;

/// Description of a shader's vertex attribute.
struct AUTO_API VertexAttribute
{
    /// Name of attribute.
    String _name;
    /// Attribute binding point. 
    unsigned _location;
    /// Attribute semantic.
    ElementSemantic _semantic;
    /// Attribute's semantic index.
    unsigned char _index;
};

/// Linked shader program consisting of vertex and pixel shaders.
class AUTO_API ShaderProgram : public GPUObject
{
public:
    /// Construct with shader pointers.
    ShaderProgram(ShaderVariation* vs, ShaderVariation* ps);
    /// Destruct.
    ~ShaderProgram();

    /// Release the linked shader program.
    void Release() override;

    /// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
    bool Link();

    /// Return the vertex shader.
    ShaderVariation* VertexShader() const;
    /// Return the pixel shader.
    ShaderVariation* PixelShader() const;
    /// Return vertex attribute descriptions.
    const Vector<VertexAttribute>& Attributes() const { return _attributes; }
    /// Return combined name of the shader program.
    String FullName() const;

    /// Return the OpenGL shader program identifier. Used internally and should not be called by portable application code.
    unsigned GLProgram() const { return _program; }

private:
    /// OpenGL shader program identifier.
    unsigned _program;
    /// Vertex shader.
    WeakPtr<ShaderVariation> _vs;
    /// Pixel shader.
    WeakPtr<ShaderVariation> _ps;
    /// Vertex attribute semantics and indices.
    Vector<VertexAttribute> _attributes;
};

}
