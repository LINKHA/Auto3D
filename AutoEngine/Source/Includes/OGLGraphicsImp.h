#pragma once
#include "Auto.h"
#include "GraphicsDef.h"
#include "RenderSurface.h"

namespace Auto3D {

using SDL_GLContext = void *;

class ConstantBuffer;



/// Cached state of a frame buffer object
struct FrameBufferObject
{
	/// Frame buffer handle.
	unsigned _fbo{};
	/// Bound color attachment textures.
	SharedPtr<RenderSurface> _colorAttachments[MAX_RENDERTARGETS]{};
	/// Bound depth/stencil attachment.
	SharedPtr<RenderSurface> _depthAttachment{};
	/// Read buffer bits.
	unsigned _readBuffers{ MATH_MAX_UNSIGNED };
	/// Draw buffer bits.
	unsigned _drawBuffers{ MATH_MAX_UNSIGNED };
};



/**
* @brief : Graphics subsystem implementation. Holds API-specific objects
*/
class GraphicsImpl
{
	friend class Graphics;

public:
	/**
	* @brief : Construct
	*/
	GraphicsImpl() = default;
	/**
	* @brief : Return the GL Context
	*/
	const SDL_GLContext& GetGLContext() { return _glContext; }

private:
	/// SDL OpenGL context
	SDL_GLContext _glContext{};
	/// iOS/tvOS system framebuffer handle
	unsigned _systemFBO{};
	/// Active texture unit
	unsigned _activeTexture{};
	/// Enabled vertex attributes bitmask
	unsigned _enabledVertexAttributes{};
	/// Vertex attributes bitmask used by the current shader program
	unsigned _usedVertexAttributes{};
	/// Vertex attribute instancing bitmask for keeping track of divisors
	unsigned _instancingVertexAttributes{};
	/// Currently bound frame buffer object
	unsigned _boundFBO{};
	/// Currently bound vertex buffer object
	unsigned _boundVBO{};
	/// Currently bound uniform buffer object
	unsigned _boundUBO{};
	/// Read frame buffer for multisampled texture resolves
	unsigned _resolveSrcFBO{};
	/// Write frame buffer for multisampled texture resolves
	unsigned _resolveDestFBO{};
	/// Current pixel format
	int _pixelFormat{};
	/// OpenGL texture types in use.
	unsigned _textureTypes[MAX_TEXTURE_UNITS]{};
	/// Dirty constant buffers.
	VECTOR<SharedPtr<ConstantBuffer> > _dirtyConstantBuffers;
	/// Need FBO commit flag.
	bool _fboDirty{};
	/// Map for additional depth textures, to emulate Direct3D9 ability to mix render texture and backbuffer rendering.
	HASH_MAP<unsigned, SharedPtr<Texture2D> > _depthTextures;
	/// sRGB write mode flag.
	bool _sRGBWrite{};
	/// Map for FBO's per resolution and format.
	HASH_MAP<unsigned long long, FrameBufferObject> _frameBuffers;
	/// Need vertex attribute pointer update flag.
	bool _vertexBuffersDirty{};
	/// Last used instance data offset.
	unsigned _lastInstanceOffset{};
	/// Current mapping of vertex attribute locations by semantic. 
	//The map is owned by the shader program, so care must be 
	//taken to switch a null shader program when it's destroyed.
	PAIR_MAP<PAIR<unsigned char, unsigned char>, unsigned> _vertexAttributes;
};


}