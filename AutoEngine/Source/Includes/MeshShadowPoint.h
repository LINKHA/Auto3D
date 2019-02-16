#pragma once
#include "ShaderVariation.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class Mesh;
class Camera;
class MeshShadowPoint : public RenderComponent, public GLMeshEnable
{
public:
	explicit MeshShadowPoint(SharedPtr<Ambient> ambient);
	MeshShadowPoint(SharedPtr<Ambient> ambient,bool enable);
	~MeshShadowPoint();
	void Destory()override;
	void DrawReady()override;
	void Start()override;
	void Draw()override;
	void DrawShadow()override;
	void DisableCull() { _cullEnable = true; }
	void SetMesh(Mesh* mesh) { _mesh.reset(mesh); }
private:
	SharedPtr<ShaderVariation> _shader;
	SharedPtr<ShaderVariation> _hardShader;
	SharedPtr<Mesh> _mesh;
	bool _cullEnable;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
