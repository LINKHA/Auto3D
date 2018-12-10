#pragma once
#include "Shader.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class Mesh;
class Camera;
class MeshShadowPoint : public RenderComponent, public GLMeshEnable
{
public:
	explicit MeshShadowPoint(Ambient* ambient);
	MeshShadowPoint(Ambient* ambient,bool enable);
	~MeshShadowPoint();
	void DrawReady()override;
	void Draw()override;
	void DrawShadow()override;
	void DisableCull() { _cullEnable = true; }
	void SetMesh(Mesh* mesh) { _mesh.reset(mesh); }
private:
	Shader _shader;
	Shader _hardShader;
	SharedPtr<Mesh> _mesh;
	bool _cullEnable;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
