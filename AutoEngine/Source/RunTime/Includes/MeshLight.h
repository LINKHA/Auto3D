#pragma once
#include "RenderComponent.h"
#include "Camera.h"

namespace Auto3D {

class MeshLight : public RenderComponent , public EnableSharedFromThis<MeshLight>
{
public:
	explicit MeshLight(SharedPtr<Ambient> ambient);
	~MeshLight();
	void Destory()override;
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> _shader;

	unsigned int woodTexture;


	unsigned int _vao{};
	unsigned int _vbo{};


};


}