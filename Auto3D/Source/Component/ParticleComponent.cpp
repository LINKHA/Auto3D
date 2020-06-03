#include "Component/ParticleComponent.h"
#include <imgui.h>

namespace Auto3D
{

static const char* s_shapeNames[] =
{
	"Sphere",
	"Hemisphere",
	"Circle",
	"Disc",
	"Rect",
};

static const char* s_directionName[] =
{
	"Up",
	"Outward",
};

static const char* s_easeFuncName[] =
{
	"Linear",
	"Step",
	"SmoothStep",
	"InQuad",
	"OutQuad",
	"InOutQuad",
	"OutInQuad",
	"InCubic",
	"OutCubic",
	"InOutCubic",
	"OutInCubic",
	"InQuart",
	"OutQuart",
	"InOutQuart",
	"OutInQuart",
	"InQuint",
	"OutQuint",
	"InOutQuint",
	"OutInQuint",
	"InSine",
	"OutSine",
	"InOutSine",
	"OutInSine",
	"InExpo",
	"OutExpo",
	"InOutExpo",
	"OutInExpo",
	"InCirc",
	"OutCirc",
	"InOutCirc",
	"OutInCirc",
	"InElastic",
	"OutElastic",
	"InOutElastic",
	"OutInElastic",
	"InBack",
	"OutBack",
	"InOutBack",
	"OutInBack",
	"InBounce",
	"OutBounce",
	"InOutBounce",
	"OutInBounce",
};
BX_STATIC_ASSERT(BX_COUNTOF(s_easeFuncName) == bx::Easing::Count);


void FEmitter::create()
{
	m_shape = EEmitterShape::Sphere;
	m_direction = EEmitterDirection::Outward;

	m_handle = psCreateEmitter(m_shape, m_direction, 1024);
	m_uniforms.reset();
}

void FEmitter::destroy()
{
	psDestroyEmitter(m_handle);
}

void FEmitter::update()
{
	psUpdateEmitter(m_handle, &m_uniforms);
}

void FEmitter::imgui()
{
	//		if (ImGui::CollapsingHeader("General") )
	{
		if (ImGui::Combo("Shape", (int*)&m_shape, s_shapeNames, BX_COUNTOF(s_shapeNames))
			|| ImGui::Combo("Direction", (int*)&m_direction, s_directionName, BX_COUNTOF(s_directionName)))
		{
			psDestroyEmitter(m_handle);
			m_handle = psCreateEmitter(m_shape, m_direction, 1024);
		}

		ImGui::SliderInt("particles / s", (int*)&m_uniforms.m_particlesPerSecond, 0, 1024);

		ImGui::SliderFloat("Gravity scale"
			, &m_uniforms.m_gravityScale
			, -2.0f
			, 2.0f
		);

		ImGui::RangeSliderFloat("Life span"
			, &m_uniforms.m_lifeSpan[0]
			, &m_uniforms.m_lifeSpan[1]
			, 0.1f
			, 5.0f
		);

		if (ImGui::Button("Reset"))
		{
			psUpdateEmitter(m_handle);
		}
	}

	if (ImGui::CollapsingHeader("Position and scale"))
	{
		ImGui::Combo("Position Ease", (int*)&m_uniforms.m_easePos, s_easeFuncName, BX_COUNTOF(s_easeFuncName));

		ImGui::RangeSliderFloat("Start offset"
			, &m_uniforms.m_offsetStart[0]
			, &m_uniforms.m_offsetStart[1]
			, 0.0f
			, 10.0f
		);
		ImGui::RangeSliderFloat("End offset"
			, &m_uniforms.m_offsetEnd[0]
			, &m_uniforms.m_offsetEnd[1]
			, 0.0f
			, 10.0f
		);

		ImGui::Text("Scale:");

		ImGui::Combo("Scale Ease", (int*)&m_uniforms.m_easeScale, s_easeFuncName, BX_COUNTOF(s_easeFuncName));

		ImGui::RangeSliderFloat("Scale Start"
			, &m_uniforms.m_scaleStart[0]
			, &m_uniforms.m_scaleStart[1]
			, 0.0f
			, 3.0f
		);
		ImGui::RangeSliderFloat("Scale End"
			, &m_uniforms.m_scaleEnd[0]
			, &m_uniforms.m_scaleEnd[1]
			, 0.0f
			, 3.0f
		);
	}

	if (ImGui::CollapsingHeader("Blending and color"))
	{
		ImGui::Combo("Blend Ease", (int*)&m_uniforms.m_easeBlend, s_easeFuncName, BX_COUNTOF(s_easeFuncName));
		ImGui::RangeSliderFloat("Blend Start"
			, &m_uniforms.m_blendStart[0]
			, &m_uniforms.m_blendStart[1]
			, 0.0f
			, 1.0f
		);
		ImGui::RangeSliderFloat("Blend End"
			, &m_uniforms.m_blendEnd[0]
			, &m_uniforms.m_blendEnd[1]
			, 0.0f
			, 1.0f
		);

		ImGui::Text("Color:");

		ImGui::Combo("RGBA Ease", (int*)&m_uniforms.m_easeRgba, s_easeFuncName, BX_COUNTOF(s_easeFuncName));
		ImGui::ColorWheel("RGBA0", &m_uniforms.m_rgba[0], 0.3f);
		ImGui::ColorWheel("RGBA1", &m_uniforms.m_rgba[1], 0.3f);
		ImGui::ColorWheel("RGBA2", &m_uniforms.m_rgba[2], 0.3f);
		ImGui::ColorWheel("RGBA3", &m_uniforms.m_rgba[3], 0.3f);
		ImGui::ColorWheel("RGBA4", &m_uniforms.m_rgba[4], 0.3f);
	}
}

void FEmitter::gizmo(const float* view, const float* proj)
{
	float mtx[16];
	float scale[3] = { 1.0f, 1.0f, 1.0f };

	ImGuizmo::RecomposeMatrixFromComponents(m_uniforms.m_position, m_uniforms.m_angle, scale, mtx);

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(
		view
		, proj
		, ImGuizmo::TRANSLATE
		, ImGuizmo::LOCAL
		, mtx
	);

	ImGuizmo::DecomposeMatrixToComponents(mtx, m_uniforms.m_position, m_uniforms.m_angle, scale);
}


AParticleComponent::AParticleComponent()
{

}

AParticleComponent::~AParticleComponent()
{

}

}