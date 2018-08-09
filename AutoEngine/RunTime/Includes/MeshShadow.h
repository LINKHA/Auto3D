#pragma once
#include "Shader.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class ModelCommand;
class Camera;
class MeshShadow : public RenderComponent,public GLMeshEnable
{
public:
	explicit MeshShadow(Ambient* ambient);
	MeshShadow(Ambient* ambient, int i);
	int k;
	~MeshShadow();
	void DrawReady()override;
	void Draw()override;
	void DrawShadow()override;


private:
	Shader _shader;
	SharedPtr<ModelCommand> _model;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
