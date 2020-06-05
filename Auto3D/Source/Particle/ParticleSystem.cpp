#include "Particle/ParticleSystem.h"
#include "Renderer/ViewPass.h"
#include "Component/CameraComponent.h"
#include "Platform/ProcessWindow.h"
#include "Component/TransformComponent.h"
#include "Gameplay/Actor.h"

#include <bimg/bimg.h>
#include <bgfx/bgfx.h>
#include "RHI/bgfx_utils.h"

namespace Auto3D
{
IMPLEMENT_SINGLETON(GParticleSystem)

FEmitterSpriteHandle GParticleSystem::s_defaultSpriteHandle;

void GParticleSystem::Init()
{
	psInit();

	bimg::ImageContainer* image = imageLoad(
		"textures/particle.ktx"
		, bgfx::TextureFormat::BGRA8
	);

	s_defaultSpriteHandle = psCreateSprite(
		uint16_t(image->m_width)
		, uint16_t(image->m_height)
		, image->m_data
	);

	bimg::imageFree(image);


}

void GParticleSystem::Update(float deltaTime, ACameraComponent* camera)
{
	bgfx::ViewId viewId = ViewIdOrdinary();

	bgfx::setViewClear(viewId
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x202020ff
		, 1.0f
		, 0
	);

	GProcessWindow& processWindow = GProcessWindow::Get();

	TMatrix4x4F transposeViewMatrix = camera->GetViewMatrix().ToMatrix4().Transpose();
	TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();

	TVector3F position = camera->GetOwner()->GetTransform()->GetPosition();

	
	bgfx::setViewTransform(viewId, transposeViewMatrix.Data(), projectionMatrix.Data());
	bgfx::setViewRect(viewId, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

	psRender(viewId, transposeViewMatrix.Data(), { position._x,position._y,position._z });
	psUpdate(deltaTime);
}

void GParticleSystem::Destory()
{
	psShutdown();
}

}