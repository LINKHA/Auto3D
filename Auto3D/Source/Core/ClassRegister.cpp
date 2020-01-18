#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		;
}

}

