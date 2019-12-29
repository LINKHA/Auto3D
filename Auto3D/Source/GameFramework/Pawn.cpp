#include "Pawn.h"
#include "GameFramework/Controller.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(APawn)
	.constructor<>()
	;
}

void APawn::PossessedBy(AController* newController)
{
	const AController*  oldController = _controller;

	_controller = newController;
}

void APawn::UnPossessed()
{
	const AController* oldController = _controller;

	_controller = nullptr;

}

AController* APawn::GetController() const
{
	return _controller;
}

}