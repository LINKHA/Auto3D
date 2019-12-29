#include "Controller.h"
#include "GameFramework/Pawn.h"

namespace Auto3D
{
REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AController)
	.constructor<>()
	;
}

void AController::AttachToPawn(APawn* InPawn)
{
	//if (bAttachToPawn && RootComponent)
	//{
	//	if (InPawn)
	//	{
	//		// Only attach if not already attached.
	//		if (InPawn->GetRootComponent() && RootComponent->GetAttachParent() != InPawn->GetRootComponent())
	//		{
	//			RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	//			RootComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	//			RootComponent->AttachToComponent(InPawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//		}
	//	}
	//	else
	//	{
	//		DetachFromPawn();
	//	}
	//}
}
void AController::DetachFromPawn()
{
	/*if (bAttachToPawn && RootComponent && RootComponent->GetAttachParent() && Cast<APawn>(RootComponent->GetAttachmentRootActor()))
	{
		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}*/
}


void AController::SetPawn(APawn* inPawn)
{
	_pawn = inPawn;
	AttachToPawn(_pawn);
}

APawn* AController::GetPawn()
{
	return _pawn;
}
void AController::OnPossess(APawn* inPawn)
{
	const bool bNewPawn = GetPawn() != inPawn;

	if (inPawn != NULL)
	{
		if (bNewPawn && GetPawn())
		{
			OnUnPossess();
		}

		if (inPawn->GetController() != NULL)
		{
			inPawn->GetController()->OnUnPossess();
		}

		inPawn->PossessedBy(this);
		SetPawn(inPawn);
	}
}

void AController::OnUnPossess()
{
	if (_pawn != NULL)
	{
		_pawn->UnPossessed();
		SetPawn(NULL);
	}
}

}