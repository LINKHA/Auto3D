#pragma once
#include "RenderComponent.h"
#include "Mesh.h"
#include "ShaderVariation.h"
namespace Auto3D {
class Camera;
class InstanceBeltLine : public RenderComponent
{
public:
	InstanceBeltLine(Ambient* ambient,Mesh* mesh, Shader* shader, glm::mat4* modelMat,int count);
	~InstanceBeltLine();
	void Start()override;
	void Draw()override;
	void SetCount(int count) { _count = count; }
	void SetShader(Shader* shader) { _shader = MakeShared<ShaderVariation>(shader); }
	void SetModel(Mesh* model) { _mesh = SharedPtr<Mesh>(model); }
private:
	SharedPtr<ShaderVariation> _shader;
	SharedPtr<Mesh> _mesh;
	unsigned int _count;
	glm::mat4* _modelMatrices;
};
}
