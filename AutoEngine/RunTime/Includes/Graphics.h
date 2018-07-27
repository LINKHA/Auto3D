#pragma once
#include "GameManager.h"
#include "Ambient.h"
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
	void Clear();
	void SetColorWrite(bool enable);
	void SetDepthWrite(bool enable);
private:
	bool _colorWrite{};
	bool _depthWrite{};
};

AUTO_END
