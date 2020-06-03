#include "Particle/ParticleSystem.h"

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

void GParticleSystem::Update()
{

}

}