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
	void Draw(Camera * camera = nullptr)override;
	void SetCount(int count) { m_Count = count; }
	void SetShader(const Shader& shader) { m_Shader = shader; }
	void SetModel(const ModelCommand& model) { m_Model = model; }
private:
	Shader m_Shader;
	ModelCommand m_Model;
	unsigned int m_Count;
	glm::mat4* m_ModelMatrices;
};
AUTO_END
