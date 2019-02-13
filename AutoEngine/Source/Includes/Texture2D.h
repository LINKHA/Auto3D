#pragma once
#include "Auto.h"
#include "Texture.h"

namespace Auto3D {

class Texture2D : public Texture, public EnableSharedFromThis<Texture2D>
{
	REGISTER_OBJECT_CLASS(Texture2D, Texture)
public:
	/// Construct.
	explicit Texture2D(SharedPtr<Ambient> ambient);

	/// Load resource from stream. May be called from a worker thread. Return true if successful.
	bool BeginLoad(Deserializer& source) override;
	/// Finish resource loading. Always called from the main thread. Return true if successful.
	bool EndLoad() override;
	/// Mark the GPU resource destroyed on context destruction.
	void OnDeviceLost() override;
	/// Recreate the GPU resource and restore data if applicable.
	void OnDeviceReset() override;
	/// Release the texture.
	void Release() override;

	/**
	* @brief :Set size, format, usage and multisampling parameters for rendertargets. Zero size will follow application window size. Return true if successful.
	*		Autoresolve true means the multisampled texture will be automatically resolved to 1-sample after being rendered to and before being sampled as a texture.
	*		Autoresolve false means the multisampled texture will be read as individual samples in the shader and is not supported on Direct3D9.
	*/
	bool SetSize(int width, int height, unsigned format, TextureUsage usage = TextureUsage::Static, int multiSample = 1, bool autoResolve = true);
	/// Set data either partially or fully on a mip level. Return true if successful.
	bool SetData(unsigned level, int x, int y, int width, int height, const void* data);
	/// Set data from an image. Return true if successful. Optionally make a single channel image alpha-only.
	bool SetData(Image* image, bool useAlpha = false);

	/// Return render surface.
	SharedPtr<RenderSurface> GetRenderSurface() const { return _renderSurface; }
protected:
	/// Create the GPU texture
	bool create();
private:
	/// Image file acquired during BeginLoad
	SharedPtr<Image> _loadImage;

	/// Render surface
	SharedPtr<RenderSurface> _renderSurface;
};



}