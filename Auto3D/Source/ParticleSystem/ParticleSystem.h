#pragma once
#include <bx/allocator.h>
#include <bx/easing.h>
#include <bx/rng.h>

#include "../RHI/bounds.h"

namespace Auto3D
{

struct FEmitterHandle { uint16_t idx; };
struct FEmitterSpriteHandle { uint16_t idx; };

template<typename Ty>
inline bool isValid(Ty _handle)
{
	return _handle.idx != UINT16_MAX;
}

namespace EEmitterShape
{
	enum Data
	{
		Sphere,
		Hemisphere,
		Circle,
		Disc,
		Rect,

		Count
	};
};

namespace EEmitterDirection
{
	enum Data
	{
		Up,
		Outward,

		Count
	};
};

struct FEmitterUniforms
{
	void reset();

	float m_position[3];
	float m_angle[3];

	float m_blendStart[2];
	float m_blendEnd[2];
	float m_offsetStart[2];
	float m_offsetEnd[2];
	float m_scaleStart[2];
	float m_scaleEnd[2];
	float m_lifeSpan[2];
	float m_gravityScale;

	uint32_t m_rgba[5];
	uint32_t m_particlesPerSecond;

	bx::Easing::Enum m_easePos;
	bx::Easing::Enum m_easeRgba;
	bx::Easing::Enum m_easeBlend;
	bx::Easing::Enum m_easeScale;

	FEmitterSpriteHandle m_handle;
};

///
void psInit(uint16_t maxEmitters = 64, bx::AllocatorI* allocator = NULL);

///
void psShutdown();

///
FEmitterSpriteHandle psCreateSprite(uint16_t _width, uint16_t _height, const void* _data);

///
void psDestroy(FEmitterSpriteHandle _handle);

///
FEmitterHandle psCreateEmitter(EEmitterShape::Data _shape, EEmitterDirection::Data _direction, uint32_t _maxParticles);

///
void psUpdateEmitter(FEmitterHandle _handle, const FEmitterUniforms* _uniforms = NULL);

///
void psGetAabb(FEmitterHandle _handle, Aabb& _outAabb);

///
void psDestroyEmitter(FEmitterHandle _handle);

///
void psUpdate(float _dt);

///
void psRender(uint8_t _view, const float* _mtxView, const bx::Vec3& _eye);

}
