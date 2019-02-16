#pragma once

#include "GraphicsDef.h"

namespace Auto3D{

class Texture;

/// Color or depth-stencil surface that can be rendered into.
class RenderSurface
{
	friend class Texture2D;
	//friend class Texture2DArray;
	//friend class TextureCube;
public:
	/// Construct with parent texture.
	explicit RenderSurface(SharedPtr<Texture> parentTexture);
	/// Destruct.
	~RenderSurface();    
	/// Return parent texture.
	SharedPtr<Texture> GetParentTexture() const;
	/// Create renderbuffer that cannot be sampled as a texture. Only used on OpenGL.
	bool CreateRenderBuffer(unsigned width, unsigned height, unsigned format, int multiSample);
	/// Return OpenGL renderbuffer if created.
	unsigned GetRenderBuffer() const { return _renderBuffer; }
	/// Release surface.
	void Release();
	/// Return width.
	int GetWidth() const;

	/// Return height.
	int GetHeight() const;

	/// Return multisampling level.
	int GetMultiSample() const;
	/// Set or clear the need resolve flag. Called internally by Graphics.
	void SetResolveDirty(bool enable) { _resolveDirty = enable; }
private:
	/// Parent texture
	SharedPtr<Texture> _parentTexture;

	union
	{
		/// Direct3D12 rendertarget or depth-stencil view
		void* _renderTargetView;
		/// OpenGL renderbuffer name
		unsigned _renderBuffer;
	};

	union
	{
		/// Direct3D12 read-only depth-stencil view. Present only on depth-stencil surfaces
		void* _readOnlyView;
		/// OpenGL target
		unsigned _target;
	};
	/// Multisampled resolve dirty flag.
	bool _resolveDirty{};
};

}
