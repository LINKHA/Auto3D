#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Math/Matrix3x4.h"

namespace Auto3D
{

class AUTO_API ACameraComponent : public AActorComponent
{
	DECLARE_CLASS(ACameraComponent, AActorComponent)
public:
	ACameraComponent();

	~ACameraComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;


	/// Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
	TMatrix3x4F EffectiveWorldTransform() const;

	const TMatrix3x4F& GetViewMatrix();

private:

	/// Cached view matrix.
	mutable TMatrix3x4F _viewMatrix;
};

}