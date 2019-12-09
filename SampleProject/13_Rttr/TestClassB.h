#pragma once
#include "Adapter/AutoRttr.h"
#include "Container/Ptr.h"

#include "TestClassA.h"

namespace Ns3D
{
class Mesh : public Node
{
	RTTR_ENABLE(Node) // include the names of all direct base classes
public:
	static TSharedPtr<Mesh> CreateMesh(FString file_name)
	{
		return TSharedPtr<Mesh>(new Mesh(file_name));
	}
	virtual void Render() {}
	enum class RenderMode
	{
		POINTS,
		WIREFRAME,
		SOLID
	};
	void SetRenderMode(RenderMode mode)
	{
		_renderMode = mode;
	}
	RenderMode GetRenderMode() const
	{
		return _renderMode;
	}
protected:
	Mesh(FString name, Node* parent = nullptr)
		:Node(name,parent)
	{
	}
	RenderMode _renderMode;
};
}
