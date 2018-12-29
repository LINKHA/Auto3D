#pragma once
#include "RenderComponent.h"
#include "Mesh.h"
#include "ShaderVariation.h"
namespace Auto3D {
class Camera;
class InstanceBeltLine : public RenderComponent
{
public:
	InstanceBeltLine(SharedPtr<Ambient> ambient, SharedPtr<Mesh> mesh, SharedPtr<Shader> shader, glm::mat4* modelMat,int count);
	~InstanceBeltLine();
	void Start()override;
	void Draw()override;
	void SetCount(int count) { _count = count; }
	void SetShader(SharedPtr<Shader> shader) { _shader = MakeShared<ShaderVariation>(shader); }
	void SetModel(SharedPtr<Mesh> model) { _mesh = model; }
private:
	SharedPtr<ShaderVariation> _shader;
	SharedPtr<Mesh> _mesh;
	unsigned int _count;
	glm::mat4* _modelMatrices;
};
}
