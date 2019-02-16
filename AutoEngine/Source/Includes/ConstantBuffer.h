#pragma once

#include "Object.h"
#include "GPUObject.h"

namespace Auto3D {

class ConstantBuffer : public Object, public GPUObject
{
	REGISTER_OBJECT_CLASS(ConstantBuffer, Object)
public:
	explicit ConstantBuffer(SharedPtr<Ambient> ambient);

	/// Recreate the GPU resource and restore data if applicable.
	void OnDeviceReset() override;
	/// Release the buffer.
	void Release() override;

	/// Set size and create GPU-side buffer. Return true on success.
	bool SetSize(unsigned size);
	/// Set a generic parameter and mark buffer dirty.
	void SetParameter(unsigned offset, unsigned size, const void* data);
	/// Set a Vector3 array parameter and mark buffer dirty.
	void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);
	/// Apply to GPU.
	void Apply();

	/// Return size.
	unsigned GetSize() const { return _size; }

	/// Return whether has unapplied data.
	bool IsDirty() const { return _dirty; }
private:
	/// Shadow data.
	SharedArrayPtr<unsigned char> _shadowData;
	/// Buffer byte size.
	unsigned _size{};
	/// Dirty flag.
	bool _dirty{};

};

}