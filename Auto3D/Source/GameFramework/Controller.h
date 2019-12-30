#pragma once
#include "Engine/Components/Component.h"

namespace Auto3D
{

class AUTO_API AController : public AComponent
{
	DECLARE_CLASS(AController, AComponent)
public:
	AController();
	~AController();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	/// Get attached actor;
	AActor* GetAttachedActor();
	/// Attach to actor.Is
	void AttachToActor(AActor* actor);
	/// Unattach to actor.
	void UnAttachToActor();
	/// Is attach to pawn;
	bool IsAttachToActor() { return _isAttachToActor; }
private:
	/// If true, the controller location will match the possessed Pawn's location.
	bool _isAttachToActor;
	/// Attached actor,Use this variable to change the actor owner.
	AActor*  _attachedActor;
};

}