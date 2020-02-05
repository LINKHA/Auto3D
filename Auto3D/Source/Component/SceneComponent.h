#pragma once
#include "Component/ActorComponent.h"
#include "Resource/ResourceRef.h"

namespace Auto3D
{
class OMaterial;
class AUTO_API ASceneComponent : public AActorComponent
{
	DECLARE_A_CLASS(ASceneComponent, AActorComponent)
public:
	ASceneComponent() {}
	virtual ~ASceneComponent() {}

	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	void SetMaterial(OMaterial* material);
	OMaterial* GetMaterial() const;
private:
	OMaterial* _material;
};

}