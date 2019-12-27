#include "Actor.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AActor)
	.constructor<>()
	;
}
void AActor::BeginPlay()
{

}

void AActor::Tick(float DeltaSeconds)
{

}

}