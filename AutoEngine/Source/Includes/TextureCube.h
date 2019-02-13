#pragma once
#include "Texture.h"

namespace Auto3D {

class TextureCube : public Texture
{
	REGISTER_OBJECT_CLASS(TextureCube, Texture)
public:
	/// Construct
	explicit TextureCube(SharedPtr<Ambient> ambient);
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
protected:
	bool create() override;
};

}