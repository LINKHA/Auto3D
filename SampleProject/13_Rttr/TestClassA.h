#pragma once
#include "Adapter/AutoRttr.h"
#include "Container/String.h"
#include "Container/Ptr.h"
#include "Container/Vector.h"

#include "Math/AutoMath.h"

using namespace Auto3D;

namespace Ns3D
{

class Node : public FRefCounted
{
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
public:
	Node(FString name, Node* parent = nullptr):
		_name(name),
		_parent(parent)
	{}
	virtual ~Node() {}
	void SetName(const FString& name)
	{
		_name = name;
	}
	const FString& GetName() const
	{
		return _name;
	}
	TVector<Node*> GetChildren() const
	{
		return _children;
	}
	void SetVisible(bool visible, bool cascade = true) {}
	virtual void Render() {}
private:
	Node* _parent;
	FString _name;
	TVector<Node*> _children;

	bool boolV;
	char charV;	
	unsigned unsignedV;	
	int intV;
	TVector2I tVector2IV;
	TRectI tRectIV;
	float floatV;
	TVector2F tVector2FV;
	FQuaternion fQuaternionV;
	FColor fColorV;
	TRectF tRectFV;

};

}