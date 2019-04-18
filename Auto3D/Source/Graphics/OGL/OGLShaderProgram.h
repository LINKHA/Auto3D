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
    ElementSemantic::Type _semantic;
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

	void SetBool(const String& name, bool value) const;
	void SetInt(const String& name, int value) const;
	void SetFloat(const String& name, float value) const;
	void SetVec2(const String& name, const Vector2F& value) const;
	void SetVec2(const String& name, float x, float y) const;
	void SetVec3(const String& name, const Vector3F& value) const;
	void SetVec3(const String& name, float x, float y, float z) const;
	void SetVec4(const String& name, const Vector4F& value) const;
	void SetVec4(const String& name, float x, float y, float z, float w);
	void SetMat2(const String& name, const Matrix2x2F& mat) const;
	void SetMat3(const String& name, const Matrix3x3F& mat) const;
	void SetMat4(const String& name, const Matrix4x4F& mat) const;
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
