#include "ClassRegister.h"
//#include "Auto.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/DefaultControllerComponent.h"
#include "Component/TransformComponent.h"
#include "Component/LightComponent.h"
#include "Component/AudioListenerComponent.h"
#include "Component/AudioSourceComponent.h"

#include "Component/SpotLightComponent.h"
#include "Component/DirectionalLightComponent.h"

#include "Component/RigidBodyComponent.h"
#include "Component/ColliderBoxComponent.h"
#include "Component/ColliderCapsuleComponent.h"
#include "Component/ColliderConeComponent.h"
#include "Component/ColliderSphereComponent.h"


#include "Gameplay/World.h"
#include "Core/Object.h"
#include "Container/Vector.h"
#include "IO/Stream.h"
#include "IO/JSONFile.h"
#include "Resource/Shader.h"
#include "Resource/Material.h"
#include "Resource/Sound.h"
#include "Resource/Mesh.h"

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

	REGISTER_O_CALSS_IMP(OShader)
		.constructor<>()
		;

	REGISTER_O_CALSS_IMP(OMaterial)
		.constructor<>()
		;
	
	REGISTER_O_CALSS_IMP(OSound)
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

	REGISTER_A_CALSS_IMP(AControllerComponent)
		.constructor<>()
		;

	REGISTER_A_CALSS_IMP(ADefaultControllerComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(ATransformComponent)
		.constructor<>()
		.property("position", &ATransformComponent::GetPosition, &ATransformComponent::SetPosition)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("rotation", &ATransformComponent::GetRotation, &ATransformComponent::SetRotation)
		(
			metadata(SERIALIZABLE, true)
		)
		.property("scale", &ATransformComponent::GetScale, static_cast<void(ATransformComponent::*)(const TVector3F&)>(&ATransformComponent::SetScale))
		(
			metadata(SERIALIZABLE, true)
		)
		;

	REGISTER_A_CALSS_IMP(ALightComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AAudioListenerComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AAudioSourceComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(ARigidBodyComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AColliderComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AColliderBoxComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AColliderCapsuleComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AColliderConeComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(AColliderSphereComponent)
		.constructor<>()
		;

	REGISTER_A_CALSS_IMP(ALightComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(ASpotLightComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(ADirectionalLightComponent)
		.constructor<>()
		;
	REGISTER_A_CALSS_IMP(APointLightComponent)
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

