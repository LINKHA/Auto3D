#pragma once
#include "Auto.h"
#include "GPUObject.h"
#include "GraphicsDef.h"

namespace Auto3D {

/// Linked shader program consisting of vertex and pixel shaders.
class ShaderProgram : public GPUObject
{
public:
	/// Construct with shader pointers.
	ShaderProgram(SharedPtr<Graphics> graphics, SharedPtr<ShaderVariation> vs, SharedPtr<ShaderVariation> ps = SharedPtr<ShaderVariation>());
	/// Destruct.
	~ShaderProgram();

	/// Release the linked shader program.
	void Release() override;

	/// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
	bool Link();

	/// Return the vertex shader.
	SharedPtr<ShaderVariation> VertexShader() const;
	/// Return the pixel shader.
	SharedPtr<ShaderVariation> PixelShader() const;
	/// Return the OpenGL shader program identifier. Used internally and should not be called by portable application code.
	unsigned GLProgram() const { return _program; }

private:
	/// OpenGL shader program identifier.
	unsigned _program;
	/// Vertex shader.
	WeakPtr<ShaderVariation> _vs;
	/// Pixel shader.
	WeakPtr<ShaderVariation> _ps;
};






}