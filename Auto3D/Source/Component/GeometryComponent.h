#pragma once
#include "Component/ActorComponent.h"
#include "Resource/ResourceRef.h"
#include "Renderer/Pass.h"

namespace Auto3D
{
class OMaterial;
class FPass;
class FGeometry;

class AUTO_API AGeometryComponent : public AActorComponent
{
	DECLARE_A_CLASS(AGeometryComponent, AActorComponent)
public:
	AGeometryComponent();
	virtual ~AGeometryComponent();

	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	FPass& GetPass();

	/// Set geometry.
	void SetGeometry(FGeometry* geometry);
	/// Set material at every geometry index. Specifying null will use the default material (opaque white.)
	void SetMaterial(OMaterial* material);

public:
	/// Handle being assigned to a new parent node.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent);
private:

	FPass _pass;
};

}