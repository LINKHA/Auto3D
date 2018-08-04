#pragma once
#include "Gameobject.h"
#include "Shader.h"
#include "GLGather.h"

namespace Auto3D {

class PBR : public Component
{
public:
	explicit PBR(Ambient* ambient);
	~PBR();
	void Start()override;
	void Draw()override;
private:
	Shader m_shader;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;
	
};

}