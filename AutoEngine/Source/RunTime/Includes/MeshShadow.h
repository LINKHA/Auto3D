#pragma once
#include "Shader.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class Model;
class Camera;
class MeshShadow : public RenderComponent,public GLMeshEnable
{
public:
	explicit MeshShadow(Ambient* ambient);
	~MeshShadow();
	void DrawReady()override;
	void Draw()override;
	void DrawShadow()override;


private:
	Shader _shader;
	sharedPtr<Model> _model;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
