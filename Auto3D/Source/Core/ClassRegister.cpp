#include "ClassRegister.h"
#include "Auto.h"

namespace Auto3D
{
void FClassRegister::AutoClassRegister()
{
	REGISTER_CALSS_FACTORY_IMP(AActor)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AAudioNode)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AAudioListener)
		.constructor<>()
		.property("vel", &AAudioListener::GetVel, &AAudioListener::SetVel)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("oriAt", &AAudioListener::GetOriAt, &AAudioListener::SetOriAt)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("oriUp", &AAudioListener::GetOriUp, &AAudioListener::SetOriUp)
		(
			metadata(SERIALIZABLE, "")
			)
		;


	
	REGISTER_CALSS_FACTORY_IMP(AAudioSource)
		.constructor<>()
		.property("sound", &AAudioSource::GetSoundAttr, &AAudioSource::SetSoundAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("pitch", &AAudioSource::GetPitch, &AAudioSource::SetPitch)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("gain", &AAudioSource::GetGain, &AAudioSource::SetGain)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("vel", &AAudioSource::GetVel, &AAudioSource::SetVel)
		(
			metadata(SERIALIZABLE, "")
			)
		;
	
	REGISTER_CALSS_FACTORY_IMP(ACamera)
		.constructor<>()
		.property("nearClip", &ACamera::GetNearClip, &ACamera::SetNearClip)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("farClip", &ACamera::GetFarClip, &ACamera::SetFarClip)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("fov", &ACamera::GetFov, &ACamera::SetFov)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("aspectRatio", &ACamera::GetAspectRatio, &ACamera::SetAspectRatio)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("orthographic", &ACamera::IsOrthographic, &ACamera::SetOrthographic)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("orthoSize", &ACamera::GetOrthoSize, static_cast<void(ACamera::*)(float)>(&ACamera::SetOrthoSize))
		(
			metadata(SERIALIZABLE, "")
			)
		.property("zoom", &ACamera::GetZoom, &ACamera::SetZoom)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("lodBias", &ACamera::GetLodBias, &ACamera::SetLodBias)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("viewMask", &ACamera::GetViewMask, &ACamera::SetLayoutMask)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("ambientColor", &ACamera::GetAmbientColor, &ACamera::SetAmbientColor)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("projectionOffset", &ACamera::GetProjectionOffset, &ACamera::SetProjectionOffset)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("reflectionPlane", &ACamera::ReflectionPlaneAttr, &ACamera::SetReflectionPlaneAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("clipPlane", &ACamera::ClipPlaneAttr, &ACamera::SetClipPlaneAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("useReflection", &ACamera::GetUseReflection, &ACamera::SetUseReflection)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("useClipping", &ACamera::GetUseClipping, &ACamera::SetUseClipping)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ACamera2D)
		.constructor<>()
		.property("nearClip", &ACamera2D::GetNearClip, &ACamera2D::SetNearClip)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("farClip", &ACamera2D::GetFarClip, &ACamera2D::SetFarClip)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("fov", &ACamera2D::GetFov, &ACamera2D::SetFov)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("aspectRatio", &ACamera2D::GetAspectRatio, &ACamera2D::SetAspectRatio)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("orthographic", &ACamera2D::IsOrthographic, &ACamera2D::SetOrthographic)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("orthoSize", &ACamera2D::GetOrthoSize, static_cast<void(ACamera2D::*)(float)>(&ACamera2D::SetOrthoSize))
		(
			metadata(SERIALIZABLE, "")
			)
		.property("zoom", &ACamera2D::GetZoom, &ACamera2D::SetZoom)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("lodBias", &ACamera2D::GetLodBias, &ACamera2D::SetLodBias)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("viewMask", &ACamera2D::GetViewMask, &ACamera2D::SetLayoutMask)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("ambientColor", &ACamera2D::GetAmbientColor, &ACamera2D::SetAmbientColor)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("projectionOffset", &ACamera2D::GetProjectionOffset, &ACamera2D::SetProjectionOffset)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("reflectionPlane", &ACamera2D::ReflectionPlaneAttr, &ACamera2D::SetReflectionPlaneAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("clipPlane", &ACamera2D::ClipPlaneAttr, &ACamera2D::SetClipPlaneAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("useReflection", &ACamera2D::GetUseReflection, &ACamera2D::SetUseReflection)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("useClipping", &ACamera2D::GetUseClipping, &ACamera2D::SetUseClipping)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderChain2D)
		.constructor<>()
		;

	/*REGISTER_CALSS_IMP(AApplication)
		.constructor<>()
		;*/

	REGISTER_CALSS_FACTORY_IMP(ACollider)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ACollider2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderBox)
		.constructor<>()
		.property("size", &AColliderBox::GetSize, static_cast<void(AColliderBox::*)(const TVector3F&)>(&AColliderBox::SetSize))
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderBox2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderCapsule)
		.constructor<>()
		.property("radius", &AColliderCapsule::GetRadius, &AColliderCapsule::SetRadius)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("height", &AColliderCapsule::GetHeight, &AColliderCapsule::SetHeight)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderCircle2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderCone)
		.constructor<>()
		.property("radius", &AColliderCone::GetRadius, &AColliderCone::SetRadius)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("height", &AColliderCone::GetHeight, &AColliderCone::SetHeight)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderEdge2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AColliderSphere)
		.constructor<>()
		.property("size", &AColliderSphere::GetSize, &AColliderSphere::SetSize)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AComponent)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AController)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ADynamicModel)
		.constructor<>()
		;

	REGISTER_CALSS_IMP(AEngine)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AGeometryNode)
		.constructor<>()
		.property("materialsAttr", &AGeometryNode::GetMaterialsAttr, &AGeometryNode::SetMaterialsAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AGeometryNode2D)
		.constructor<>()
		;

	// Sprite2D cpp fail,Initializes the global constructor so apply here
	REGISTER_CALSS_FACTORY_IMP(ASprite2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AImage)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AJSONFile)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ALight)
		.constructor<>()
		.property("lightType", &ALight::GetLightTypeAttr, &ALight::SetLightTypeAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("color", &ALight::GetColor, &ALight::SetColor)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("range", &ALight::GetRange, &ALight::SetRange)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("fov", &ALight::GetFov, &ALight::SetFov)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("lightMask", &ALight::GetLightMask, &ALight::SetLightMask)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("shadowMapSize", &ALight::GetShadowMapSize, &ALight::SetShadowMapSize)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("shadowSplits", &ALight::GetShadowSplits, &ALight::SetShadowSplits)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("shadowFadeStart", &ALight::GetShadowFadeStart, &ALight::SetShadowFadeStart)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("depthBias", &ALight::GetDepthBias, &ALight::SetDepthBias)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("slopeScaledDepthBias", &ALight::GetSlopeScaledDepthBias, &ALight::SetSlopeScaledDepthBias)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AMaterial)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AIBLMaterial)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AModel)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ANode)
		.constructor<>()
		.property("name", &ANode::GetName, &ANode::SetName)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("enabled", &ANode::IsEnabled, &ANode::SetEnabled)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("temporary", &ANode::IsTemporary, &ANode::SetTemporary)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("layer", &ANode::GetLayer, &ANode::SetLayer)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("tag", &ANode::GetTag, &ANode::SetTag)
		(
			metadata(SERIALIZABLE, true)
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ANode2D)
		.constructor<>()
		.property("name", &ANode2D::GetName, &ANode2D::SetName)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("enabled", &ANode2D::IsEnabled, &ANode2D::SetEnabled)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("temporary", &ANode2D::IsTemporary, &ANode2D::SetTemporary)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("layer", &ANode2D::GetLayer, &ANode2D::SetLayer)
		(
			metadata(SERIALIZABLE, true)
			)
		.property("tag", &ANode2D::GetTag, &ANode2D::SetTag)
		(
			metadata(SERIALIZABLE, true)
			)
		;

	REGISTER_CALSS_IMP(AObject)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AOctree)
		.constructor<>()
		.property("boundingBox", &AOctree::GetBoundingBoxAttr, &AOctree::SetBoundingBoxAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("numLevels", &AOctree::GetNumLevelsAttr, &AOctree::SetNumLevelsAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AOctreeNode)
		.constructor<>()
		.property("castShadows", &AOctreeNode::GetCastShadows, &AOctreeNode::SetCastShadows)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ATexture)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(APawn)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(APhysicsWorld)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(APhysicsWorld2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AResource)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AFont)
		.constructor<>()
		.property_readonly("sdfFont", &AFont::IsSDFFont)
		;

	REGISTER_CALSS_FACTORY_IMP(ASound)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ARigidBody)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ARigidBody2D)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AScene2D)
		.constructor<>()
		.property_readonly("cameras", &AScene2D::GetCameras)
		.property("physicsWorld", &AScene2D::GetPhysicsWorld, &AScene2D::SetPhysicsWorld)
		.property("layerNames", &AScene2D::GetLayerNamesAttr, &AScene2D::SetLayerNamesAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("tagNames", &AScene2D::GetTagNamesAttr, &AScene2D::SetTagNamesAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AShader)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(ASkyBox)
		.constructor<>()
		.property("mapSize", &ASkyBox::GetMapSize, &ASkyBox::SetMapSize)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("irradianceSize", &ASkyBox::GetIrradianceSize, &ASkyBox::SetIrradianceSize)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("prefilterSize", &ASkyBox::GetPrefilterSize, &ASkyBox::SetPrefilterSize)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AStaticModel)
		.constructor<>()
		.property("modelAttr", &AStaticModel::GetModelAttr, &AStaticModel::SetModelAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("lodBias", &AStaticModel::GetLodBias, &AStaticModel::SetLodBias)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ATransform)
		.constructor<>()
		.property("position", &ATransform::GetPosition, &ATransform::SetPosition)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("rotation", &ATransform::GetRotation, &ATransform::SetRotation)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("scale", &ATransform::GetScale, static_cast<void(ATransform::*)(const TVector3F&)>(&ATransform::SetScale))
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ATransform2D)
		.constructor<>()
		.property("position", &ATransform2D::GetPosition, &ATransform2D::SetPosition)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("rotation", &ATransform2D::GetRotation, &ATransform2D::SetRotation)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("scale", &ATransform2D::GetScale, static_cast<void(ATransform2D::*)(const TVector3F&)>(&ATransform2D::SetScale))
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(AWater)
		.constructor<>()
		;

	REGISTER_CALSS_FACTORY_IMP(AWorld)
		.constructor<>()
		.property_readonly("cameras", &AWorld::GetCameras)
		.property("physicsWorld", &AWorld::GetPhysicsWorld, &AWorld::SetPhysicsWorld)
		.property("skybox", &AWorld::GetSkyBox, &AWorld::SetSkyBox)
		.property("shadowMapAttr", &AWorld::GetShadowMapAttr, &AWorld::SetupShadowMapAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("layerNames", &AWorld::GetLayerNamesAttr, &AWorld::SetLayerNamesAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		.property("tagNames", &AWorld::GetTagNamesAttr, &AWorld::SetTagNamesAttr)
		(
			metadata(SERIALIZABLE, "")
			)
		;

	REGISTER_CALSS_FACTORY_IMP(ADefaultController)
		.constructor<>()
		;
}
}

