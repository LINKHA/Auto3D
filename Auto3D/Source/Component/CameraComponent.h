#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Math/Matrix3x4.h"

namespace Auto3D
{

class AUTO_API ACameraComponent : public AActorComponent
{
	DECLARE_A_CLASS(ACameraComponent, AActorComponent)
public:
	ACameraComponent();

	~ACameraComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	void envViewMtx(float* _mtx);

	/// Set near clip distance.
	void SetNearClip(float nearClip);
	/// Set far clip distance.
	void SetFarClip(float farClip);
	/// Set vertical field of view in degrees.
	void SetFov(float fov);
	/// Set aspect ratio.
	void SetAspectRatio(float aspectRatio);
	/// Return far clip distance.
	float GetFarClip() const { return _farClip; }
	/// Return near clip distance.
	float GetNearClip() const { return _nearClip; }
	/// Return vertical field of view in degrees.
	float GetFov() const { return _fov; }
	/// Return aspect ratio.
	float GetAspectRatio() const { return _aspectRatio; }

	/// Set view layer mask. Will be checked against scene objects' layers to see what to render.
	void SetLayoutMask(unsigned mask);
	/// Set view layer mask. Will be checked against scene objects' layers to see what to render.
	void SetLayoutMaskIndex(unsigned maskIndex);
	/// Set view layer mask from name. Will be checked against scene objects' layers to see what to render.
	void SetLayoutMaskName(const FString& name);
	/// Set view layer out with mask.
	void SetLayoutMaskOutIndex(unsigned maskIndex);
	/// Set view layer out with mask.
	void SetLayoutMaskOutName(const FString& name);
	/// Set view layer mask all layout.
	void SetLayoutMaskAll();
	/// Return view layer mask.
	unsigned GetViewMask() const { return _viewLayoutMask; }

	/// Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
	TMatrix3x4F EffectiveWorldTransform() const;
	/// Return view matrix.
	const TMatrix3x4F& GetViewMatrix();
	/// Return either API-specific or API-independent (D3D convention) projection matrix.
	TMatrix4x4F GetProjectionMatrix(bool apiSpecific = true) const;
private:
	/// Cached view matrix.
	mutable TMatrix3x4F _viewMatrix;

	/// Near clip distance.
	float _nearClip;
	/// Far clip distance.
	float _farClip;
	/// Field of view.
	float _fov;
	/// Aspect ratio.
	float _aspectRatio;

	/// View layer mask.
	unsigned _viewLayoutMask;
};

}