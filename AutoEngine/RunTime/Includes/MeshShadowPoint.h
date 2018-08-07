#pragma once
#include "Shader.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class ModelCommand;
class Camera;
class MeshShadowPoint : public RenderComponent, public GLMeshEnable
{
public:
	explicit MeshShadowPoint(Ambient* ambient);
	MeshShadowPoint(Ambient* ambient, int i);
	int k;
	~MeshShadowPoint();
	void DrawReady()override;
	void Draw()override;
	void DrawShadow()override;
private:
	Shader _shader;
	ModelCommand* _model;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
