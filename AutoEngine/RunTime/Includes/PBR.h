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
	void Draw()override;
private:
	Shader m_shader;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;
	
};

AUTO_END