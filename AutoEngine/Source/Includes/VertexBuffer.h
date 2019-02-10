#pragma once
#include "Auto.h"
#include "Object.h"
#include "GPUObject.h"

namespace Auto3D{

class VertexBuffer : public Object , public GPUObject
{
	REGISTER_OBJECT_CLASS(VertexBuffer, Object)
public:
	/**
	* @brief : Construct. Optionally force headless (no GPU-side buffer) operation
	*/
	explicit VertexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless = false);
	/**
	* @brief : Mark the buffer destroyed on graphics context destruction. May be a no-op depending on the API
	*/
	void OnDeviceLost() override;
	/**
	* @brief : Recreate the buffer and restore data if applicable. May be a no-op depending on the API
	*/
	void OnDeviceReset() override;
	/**
	* @brief : Release buffer
	*/
	void Release() override;
	/**
	* @brief : Enable shadowing in CPU memory. Shadowing is forced on if the graphics subsystem does not exist.
	*/
	void SetShadowed(bool enable);

	/**
	* @brief : Return whether CPU memory shadowing is enabled
	*/
	bool IsShadowed() const { return _shadowed; }
	
	/**
	* @brief : Set size, vertex elements and dynamic mode. Previous data will be lost
	*/
	bool SetSize(unsigned vertexCount, unsigned elementMask, bool dynamic);
	/**
	* @brief : Set size, vertex elements and dynamic mode. Previous data will be lost
	*/
	bool VertexBuffer::SetSize(unsigned vertexCount, const VECTOR<VertexElement>& elements, bool dynamic);
	/**
	* @brief : Set all data in the buffer
	*/
	bool SetData(const void* data);

	/**
	*@brief :Return legacy vertex element mask.
	*	Note that both semantic and type must match
	*	the legacy element for a mask bit to be set.
	*/
	VertexMaskFlags GetElementMask() const { return _elementMask; }
	/**
	* @brief : Return a vertex element list from a legacy element bitmask
	*/
	static VECTOR<VertexElement> GetElements(unsigned elementMask);

private:
	/**
	* @brief : Create buffer
	*/
	bool create();
	/**
	* @brief : Update offsets of vertex elements
	*/
	void updateOffsets();
private:

	/// Shadow data
	SharedArrayPtr<unsigned char> _shadowData;
	/// Number of vertices
	unsigned _vertexCount{};
	/// Vertex size
	unsigned _vertexSize{};
	/// Vertex elements
	VECTOR<VertexElement> _elements;
	/// Vertex element hash.
	unsigned long long _elementHash{};
	/// Vertex element legacy bitmask.
	VertexMaskFlags _elementMask{};
	/// Dynamic flag
	bool _dynamic{};
	/// Shadowed flag
	bool _shadowed{};
};

}