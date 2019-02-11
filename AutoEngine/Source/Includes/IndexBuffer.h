#pragma once
#include "Object.h"
#include "GPUObject.h"
#include "GraphicsDef.h"

namespace Auto3D
{
/**
* Hardware index buffer
*/
class IndexBuffer : public Object, public GPUObject, public EnableSharedFromThis<IndexBuffer>
{
	REGISTER_OBJECT_CLASS(IndexBuffer, Object)
public:
	/// Construct. Optionally force headless (no GPU-side buffer) operation.
	explicit IndexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless = false);

	/// Mark the buffer destroyed on graphics context destruction. May be a no-op depending on the API.
	void OnDeviceLost() override;
	/// Recreate the buffer and restore data if applicable. May be a no-op depending on the API.
	void OnDeviceReset() override;
	/// Release buffer.
	void Release() override;

	/// Enable shadowing in CPU memory. Shadowing is forced on if the graphics subsystem does not exist.
	void SetShadowed(bool enable);
	/// Set size and vertex elements and dynamic mode. Previous data will be lost.
	bool SetSize(unsigned indexCount, bool largeIndices, bool dynamic = false);
	/// Set all data in the buffer.
	bool SetData(const void* data);

private:
	/// Create buffer
	bool create();
private:
	/// Shadow data
	SharedArrayPtr<unsigned char> _shadowData;
	/// Number of indices
	unsigned _indexCount{};
	/// Index size
	unsigned _indexSize{};
	/// Lock start vertex
	unsigned _lockStart{};
	/// Lock number of vertices
	unsigned _lockCount{};
	/// Dynamic flag
	bool _dynamic{};
	/// Shadowed flag
	bool _shadowed{};
};

}
