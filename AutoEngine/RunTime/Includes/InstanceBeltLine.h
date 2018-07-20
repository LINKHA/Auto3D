#pragma once
#include "GameObject.h"
#include "ModelCommand.h"
#include "Shader.h"
AUTO_BEGIN
class Camera;
class InstanceBeltLine : public Component
{
public:
	InstanceBeltLine(const ModelCommand& model,const Shader& shader, glm::mat4* modelMat,int count);
	~InstanceBeltLine();
	void Start()override;
	void Draw()override;
	void SetCount(int count) { _count = count; }
	void SetShader(const Shader& shader) { _shader = shader; }
	void SetModel(const ModelCommand& model) { _model = model; }
private:
	Shader _shader;
	ModelCommand _model;
	unsigned int _count;
	glm::mat4* _modelMatrices;
};
AUTO_END
