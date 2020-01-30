#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Gameplay/World.h"
#include "Core/Object.h"

using namespace Auto3D;

static void REGISTER_REFLECTION_FUNCATION()
{
	REGISTER_CALSS_IMP(OObject)
		.constructor<>()
		.method("getTypeName", &OObject::GetTypeName)
		;

	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		.property("id", &AActor::GetId, &AActor::SetId)
		.property("name", &AActor::GetName, &AActor::SetName)
		.property("childrenNode", &AActor::_children, registration::protected_access)
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


namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_REFLECTION_FUNCATION();
}

}

