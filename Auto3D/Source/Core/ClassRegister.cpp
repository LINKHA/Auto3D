#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Gameplay/World.h"
#include "Core/Object.h"

namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_CALSS_IMP(OObject)
		.constructor<>()
		.method("getTypeName", &OObject::GetTypeName)
		;

	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		.property("id", &AActor::GetId, &AActor::SetId)
		.property("name", &AActor::GetName, &AActor::SetName)
	;

	REGISTER_CALSS_IMP(AWorld)
		.constructor<>()
		;

	REGISTER_CALSS_IMP(ACameraComponent)
		.constructor<>()
		;

	REGISTER_CALSS_IMP(AMeshComponent)
		.constructor<>()
		;
}

}

