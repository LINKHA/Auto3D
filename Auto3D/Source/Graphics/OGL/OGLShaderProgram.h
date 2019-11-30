#pragma once

#include "Container/String.h"
#include "Graphics/GPUObject.h"
#include "Graphics/GraphicsDefs.h"

namespace Auto3D
{

class AGraphics;
class FShaderVariation;

/// Description of a shader's vertex attribute.
struct AUTO_API FVertexAttribute
{
    /// FName of attribute.
    FString _name;
    /// FAttribute binding point. 
    unsigned _location;
    /// FAttribute semantic.
    EElementSemantic::Type _semantic;
    /// FAttribute's semantic index.
    unsigned char _index;
};

/// Linked shader program consisting of vertex and pixel shaders.
class AUTO_API FShaderProgram : public FGPUObject
{
public:
    /// Construct with shader pointers.
    FShaderProgram(FShaderVariation* vs, FShaderVariation* ps);
    /// Destruct.
    ~FShaderProgram();

    /// Release the linked shader program.
    void Release() override;

    /// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
    bool Link();

    /// Return the vertex shader.
    FShaderVariation* VertexShader() const;
    /// Return the pixel shader.
    FShaderVariation* PixelShader() const;
    /// Return vertex attribute descriptions.
    const TVector<FVertexAttribute>& Attributes() const { return _attributes; }
    /// Return combined name of the shader program.
    FString FullName() const;

    /// Return the OpenGL shader program identifier. Used internally and should not be called by portable application code.
    unsigned GLProgram() const { return _program; }

	void SetBool(const FString& name, bool value) const;
	void SetInt(const FString& name, int value) const;
	void SetFloat(const FString& name, float value) const;

	void SetVec2(const FString& name, const Vector2F& value) const;
	void SetVec2(const FString& name, float x, float y) const;
	void SetVec2s(const FString& name, int size, float* pVec2) const;

	void SetVec3(const FString& name, const Vector3F& value) const;
	void SetVec3(const FString& name, float x, float y, float z) const;
	void SetVec3s(const FString& name, int size, float* pVec3) const;

	void SetVec4(const FString& name, const Vector4F& value) const;
	void SetVec4(const FString& name, float x, float y, float z, float w);
	void SetVec4s(const FString& name, int size, float* pVec4);

	void SetMat2(const FString& name, const Matrix2x2F& mat) const;
	void SetMat3(const FString& name, const Matrix3x3F& mat) const;
	void SetMat4(const FString& name, const Matrix4x4F& mat) const;

	unsigned GetUniformLocation(const FString& uniformName) const;
	unsigned GetAttribLocation(const FString& AttribName) const;
private:
    /// OpenGL shader program identifier.
    unsigned _program;
    /// Vertex shader.
    TWeakPtr<FShaderVariation> _vs;
    /// Pixel shader.
    TWeakPtr<FShaderVariation> _ps;
    /// Vertex attribute semantics and indices.
    TVector<FVertexAttribute> _attributes;
};

}
