#pragma once
#include "GameFramework/Actor.h"

namespace Auto3D
{
class APawn;

class AUTO_API AController : public AActor
{
	DECLARE_CLASS(AController,AActor)
public:
	AController() {}
	~AController() {}
	
	/// Physically attach the Controller to the specified Pawn, so that our position reflects theirs.
	/// The attachment persists during possession of the pawn. The Controller's rotation continues to match the ControlRotation.
	/// Attempting to attach to a nullptr Pawn will call DetachFromPawn() instead.
	virtual void AttachToPawn(APawn* inPawn);

	/// Detach the RootComponent from its parent, but only if bAttachToPawn is true and it was attached to a Pawn.
	virtual void DetachFromPawn();

	/// Setter for Pawn. Normally should only be used internally when possessing/unpossessing a Pawn.
	virtual void SetPawn(APawn* inPawn);
	/// Return the Pawn that is currently 'controlled' by this PlayerController. 
	APawn* GetPawn();
	/// Overridable native function for when this controller possesses a pawn.
	virtual void OnPossess(APawn* inPawn);
	/// Overridable native function for when this controller unpossesses its pawn.
	virtual void OnUnPossess();

private:
	// If true, the controller location will match the possessed Pawn's location.
	bool _isAttachToPawn;

	APawn* _pawn;
};

}