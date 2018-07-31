#pragma once
#include "GameObject.h"
#include "Shader.h"
namespace Auto3D {
class PBRMaterial: public Component
{
public:
	explicit PBRMaterial(Ambient* ambient);
	~PBRMaterial();
	void Start()override;
	void Draw()override;
private:
	Shader pbrShader;


	unsigned int sphereVAO = 0;
	unsigned int indexCount;


};

}