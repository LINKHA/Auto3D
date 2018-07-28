#pragma once
#include "GameManager.h"
#include "Ambient.h"
#include "Math/Color.h"
AUTO_BEGIN
class Graphics : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Graphics, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Graphics);
public:
	explicit Graphics(Ambient* ambient);
	void Init();

	bool BeginFrame();
	void EndFrame();	
	void Clear(unsigned flags , const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
	void SetColorWrite(bool enable);
	void SetDepthWrite(bool enable);
private:
	bool _colorWrite{};
	bool _depthWrite{};
	unsigned _stencilWriteMask{};
};

AUTO_END
