#pragma once
#include "Auto.h"
#include "GraphicsDef.h"

namespace Auto3D {

using SDL_GLContext = void *;
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
	/// Currently bound frame buffer object.
	unsigned boundFBO_{};
	/// Currently bound vertex buffer object.
	unsigned boundVBO_{};
	/// Currently bound uniform buffer object.
	unsigned boundUBO_{};
	/// Read frame buffer for multisampled texture resolves.
	unsigned resolveSrcFBO_{};
	/// Write frame buffer for multisampled texture resolves.
	unsigned resolveDestFBO_{};
	/// Current pixel format.
	int pixelFormat_{};
};


}