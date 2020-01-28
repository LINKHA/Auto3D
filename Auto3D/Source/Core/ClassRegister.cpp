#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Gameplay/World.h"

namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		.method("getType", &AActor::GetTypeName)
		.property("id", &AActor::GetId, &AActor::SetId)
		.property("name", &AActor::GetName, &AActor::SetName)
	;

	REGISTER_CALSS_IMP(AWorld)
		.constructor<>()
		.method("getType", &AWorld::GetTypeName)
		;

	REGISTER_CALSS_IMP(ACameraComponent)
		.constructor<>()
		;

	REGISTER_CALSS_IMP(AMeshComponent)
		.constructor<>()
		;
}

}

