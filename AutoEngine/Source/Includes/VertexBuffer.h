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
	* @brief : Set size, vertex elements and dynamic mode. Previous data will be lost
	*/
	bool SetSize();
	/**
	* @brief : Set all data in the buffer
	*/
	bool SetData(const void* data);


private:
	/**
	* @brief : Create buffer
	*/
	void create();

private:
	/// Number of vertices
	unsigned _vertexCount{};
	/// Vertex size
	unsigned _vertexSize{};
};

}