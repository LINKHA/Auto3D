#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Gameplay/World.h"
#include "Core/Object.h"
#include "Container/Vector.h"
#include "IO/Stream.h"
#include "IO/JSONFile.h"


using namespace Auto3D;

static void REGISTER_REFLECTION_FUNCATION()
{
	REGISTER_CALSS_IMP(OObject)
		.constructor<>()
		.method("getTypeName", &OObject::GetTypeName)
		;

	REGISTER_CALSS_IMP(AActor)
		.constructor<>()
		.property("name", &AActor::GetName, &AActor::SetName)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("enabled", &AActor::IsEnabled, &AActor::SetEnabled)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("temporary", &AActor::IsTemporary, &AActor::SetTemporary)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("layer", &AActor::GetLayer, &AActor::SetLayer)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("tag", &AActor::GetTag, &AActor::SetTag)
		(
			metadata(SERIALIZABLE, true)
		)
		;

	REGISTER_CALSS_IMP(AWorld)
		.constructor<>()
		.property("layerNames", &AWorld::GetLayerNamesAttr, &AWorld::SetLayerNamesAttr)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("tagNames", &AWorld::GetTagNamesAttr, &AWorld::SetTagNamesAttr)
		(
			metadata(SERIALIZABLE, true)
		)
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

