#pragma once
#include "Gameobject.h"
#include "Shader.h"
#include "OpenGLGather.h"

AUTO_BEGIN

class PBR : public Component
{
public:
	PBR();
	~PBR();
	void Start()override;
	void Draw(Camera * camera = nullptr)override;
private:
	Shader m_shader;
	
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
};

AUTO_END