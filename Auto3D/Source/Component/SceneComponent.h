#pragma once
#include "Component/ActorComponent.h"
#include "Resource/ResourceRef.h"
#include "Renderer/Pass.h"

namespace Auto3D
{
class OMaterial;
class FPass;

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

	void SetGeometryName(const FString& name);
	const FPass& GetPass();

private:
	FPass _pass;

	OMaterial* _material;
};

}