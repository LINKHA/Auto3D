#pragma once

#include "GraphicsDef.h"

namespace Auto3D{

class Texture;

/// Color or depth-stencil surface that can be rendered into.
class RenderSurface
{
	friend class Texture2D;

public:
	/// Construct with parent texture.
	explicit RenderSurface(Texture* parentTexture);
	/// Destruct.
	~RenderSurface() override;

	/// Set number of viewports.
	void SetNumViewports(unsigned num);
	/// Set viewport.
	void SetViewport(unsigned index, Viewport* viewport);
	/// Set viewport update mode. Default is to update when visible.
	void SetUpdateMode(RenderSurfaceUpdateMode mode);
	/// Set linked color rendertarget.
	void SetLinkedRenderTarget(RenderSurface* renderTarget);
	/// Set linked depth-stencil surface.
	void SetLinkedDepthStencil(RenderSurface* depthStencil);
	/// Queue manual update of the viewport(s).
	void QueueUpdate();
	/// Release surface.
	void Release();
	/// Mark the GPU resource destroyed on graphics context destruction. Only used on OpenGL.
	void OnDeviceLost();
	/// Create renderbuffer that cannot be sampled as a texture. Only used on OpenGL.
	bool CreateRenderBuffer(unsigned width, unsigned height, unsigned format, int multiSample);

	/// Return width.
	int GetWidth() const;

	/// Return height.
	int GetHeight() const;

	/// Return usage.
	TextureUsage GetUsage() const;

	/// Return multisampling level.
	int GetMultiSample() const;

	/// Return multisampling autoresolve mode.
	bool GetAutoResolve() const;

	/// Return number of viewports.
	unsigned GetNumViewports() const { return viewports_.Size(); }

	/// Return viewport by index.
	Viewport* GetViewport(unsigned index) const;

	/// Return viewport update mode.
	RenderSurfaceUpdateMode GetUpdateMode() const { return updateMode_; }

	/// Return linked color rendertarget.
	RenderSurface* GetLinkedRenderTarget() const { return linkedRenderTarget_; }

	/// Return linked depth-stencil surface.
	RenderSurface* GetLinkedDepthStencil() const { return linkedDepthStencil_; }

	/// Return whether manual update queued. Called internally.
	bool IsUpdateQueued() const { return updateQueued_; }

	/// Reset update queued flag. Called internally.
	void ResetUpdateQueued();

	/// Return parent texture.
	Texture* GetParentTexture() const { return parentTexture_; }

	/// Return Direct3D9 surface.
	void* GetSurface() const { return surface_; }

	/// Return Direct3D11 rendertarget or depth-stencil view. Not valid on OpenGL.
	void* GetRenderTargetView() const { return renderTargetView_; }

	/// Return Direct3D11 read-only depth-stencil view. May be null if not applicable. Not valid on OpenGL.
	void* GetReadOnlyView() const { return readOnlyView_; }

	/// Return surface's OpenGL target.
	unsigned GetTarget() const { return target_; }

	/// Return OpenGL renderbuffer if created.
	unsigned GetRenderBuffer() const { return renderBuffer_; }

	/// Return whether multisampled rendertarget needs resolve.
	bool IsResolveDirty() const { return resolveDirty_; }

	/// Set or clear the need resolve flag. Called internally by Graphics.
	void SetResolveDirty(bool enable) { resolveDirty_ = enable; }

private:
	/// Parent texture.
	Texture* parentTexture_;

	union
	{
		/// Direct3D9 surface.
		void* surface_;
		/// Direct3D11 rendertarget or depth-stencil view.
		void* renderTargetView_;
		/// OpenGL renderbuffer name.
		unsigned renderBuffer_;
	};

	union
	{
		/// Direct3D11 read-only depth-stencil view. Present only on depth-stencil surfaces.
		void* readOnlyView_;
		/// OpenGL target.
		unsigned target_;
	};

	/// Viewports.
	Vector<SharedPtr<Viewport> > viewports_;
	/// Linked color buffer.
	WeakPtr<RenderSurface> linkedRenderTarget_;
	/// Linked depth buffer.
	WeakPtr<RenderSurface> linkedDepthStencil_;
	/// Update mode for viewports.
	RenderSurfaceUpdateMode updateMode_{ SURFACE_UPDATEVISIBLE };
	/// Update queued flag.
	bool updateQueued_{};
	/// Multisampled resolve dirty flag.
	bool resolveDirty_{};
};

}
