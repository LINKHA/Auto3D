#include "InstanceBeltLine.h"
#include "ModelCommand.h"
#include "Math/Rect.h"
#include "GLWindow.h"
#include "Camera.h"
#include "RenderManager.h"
AUTO_BEGIN

InstanceBeltLine::InstanceBeltLine(const ModelCommand& model, const Shader& shader, glm::mat4* modelMat,int count)
	:m_Model(model)
	,m_Shader(shader)
	,m_ModelMatrices(modelMat)
	,m_Count(count)
{
}
InstanceBeltLine::~InstanceBeltLine()
{
}
void InstanceBeltLine::Start()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < m_Model.meshes.size(); i++)
	{
		unsigned int VAO = m_Model.meshes[i].VAO;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}
void InstanceBeltLine::Draw()
{
	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();

	glm::mat4 projectionMat = INSTANCE(RenderManager).GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 viewMat = INSTANCE(RenderManager).GetCurrentCamera().GetViewMatrix();

	m_Shader.Use();
	m_Shader.SetMat4("projection", projectionMat);
	m_Shader.SetMat4("view", viewMat);
	m_Shader.SetInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Model.textures_loaded[0].id); 
	for (unsigned int i = 0; i < m_Model.meshes.size(); i++)
	{
		glBindVertexArray(m_Model.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, m_Model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, m_Count);
		glBindVertexArray(0);
	}

}
AUTO_END