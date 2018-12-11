#pragma once
#include "RenderComponent.h"
#include "Camera.h"
#include "_Shader.h"

namespace Auto3D {

class MeshLight : public RenderComponent
{
public:
	explicit MeshLight(Ambient* ambient);
	~MeshLight();
	void Start()override;
	void Draw()override;
private:
	_Shader _shader;

	unsigned int woodTexture;


	unsigned int _vao{};
	unsigned int _vbo{};


};


}