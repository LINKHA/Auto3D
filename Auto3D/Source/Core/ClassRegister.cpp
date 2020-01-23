#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"

namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		;

	/*REGISTER_CALSS_IMP(ACameraComponent)
		.constructor<>()
		;*/

	
}

}

