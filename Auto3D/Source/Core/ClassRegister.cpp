#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/DefaultController.h"
#include "Component/Transform.h"
#include "Resource/Mesh.h"

#include "Gameplay/World.h"
#include "Core/Object.h"
#include "Container/Vector.h"
#include "IO/Stream.h"
#include "IO/JSONFile.h"


using namespace Auto3D;

static void REGISTER_REFLECTION_FUNCATION()
{
	REGISTER_O_CALSS_IMP(OObject)
		.constructor<>()
		.method("getTypeName", &OObject::GetTypeName)
		;

	REGISTER_O_CALSS_IMP(OResource)
		.constructor<>()
		;

	REGISTER_O_CALSS_IMP(OMesh)
		.constructor<>()
		;

	REGISTER_A_CALSS_IMP(AActor)
		.constructor<>()
		.property("name", &AActor::GetPathName, &AActor::SetName)
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

	REGISTER_A_CALSS_IMP(AWorld)
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

	REGISTER_A_CALSS_IMP(ACameraComponent)
		.constructor<>()
		.property("nearClip", &ACameraComponent::GetNearClip, &ACameraComponent::SetNearClip)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("farClip", &ACameraComponent::GetFarClip, &ACameraComponent::SetFarClip)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("fov", &ACameraComponent::GetFov, &ACameraComponent::SetFov)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("aspectRatio", &ACameraComponent::GetAspectRatio, &ACameraComponent::SetAspectRatio)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("viewLayoutMask", &ACameraComponent::GetViewMask, &ACameraComponent::SetLayoutMask)
		(
			metadata(SERIALIZABLE, true)
		)
		;

	REGISTER_A_CALSS_IMP(AMeshComponent)
		.constructor<>()
		.property("mesh", &AMeshComponent::GetMeshAttr, &AMeshComponent::SetMeshAttr)
		(
			metadata(SERIALIZABLE, true)
		)
		;

	REGISTER_A_CALSS_IMP(AController)
		.constructor<>()
		;

	REGISTER_A_CALSS_IMP(ADefaultController)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(ATransform)
		.constructor<>()
		.property("position", &ATransform::GetPosition, &ATransform::SetPosition)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("rotation", &ATransform::GetRotation, &ATransform::SetRotation)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("scale", &ATransform::GetScale, static_cast<void(ATransform::*)(const TVector3F&)>(&ATransform::SetScale))
		(
			metadata(SERIALIZABLE, true)
		)
		;

}


namespace Auto3D
{

void FClassRegister::AutoClassRegister()
{
	REGISTER_REFLECTION_FUNCATION();
}

}

