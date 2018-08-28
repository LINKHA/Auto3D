#pragma once
#include "RenderComponent.h"
#include "Model.h"
#include "Shader.h"
namespace Auto3D {
class Camera;
class InstanceBeltLine : public RenderComponent
{
public:
	InstanceBeltLine(Ambient* ambient,const Model& model,const Shader& shader, glm::mat4* modelMat,int count);
	~InstanceBeltLine();
	void Start()override;
	void Draw()override;
	void SetCount(int count) { _count = count; }
	void SetShader(const Shader& shader) { _shader = shader; }
	void SetModel(const Model& model) { _model = model; }
private:
	Shader _shader;
	Model _model;
	unsigned int _count;
	glm::mat4* _modelMatrices;
};
}
