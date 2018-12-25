#include "DeferredShading.h"
#include "Graphics.h"
#include "Camera.h"
#include "Renderer.h"
#include "BaseMesh.h"
#include "ResourceSystem.h"
#include "NewDef.h"
namespace Auto3D {
DeferredShading::DeferredShading(Ambient* ambient)
	: RenderComponent(ambient)
{
	auto* cach = GetSubSystem<ResourceSystem>();
	m_shaderGeometryPass = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_g_buffer.glsl"));
	m_shaderGeometryPass->Create();

	m_shaderLightingPass = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_deffered_shading.glsl"));
	m_shaderLightingPass->Create();

	m_shaderLightBox = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_deffered_light_box.glsl"));
	m_shaderLightBox->Create();
}
DeferredShading::~DeferredShading()
{
	UnloadOpaque(this);
}
void DeferredShading::Start()
{
	auto* mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/nanosuit/nanosuit.obj");
	nanosuit = SharedPtr<Mesh>(mesh);
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));

	RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();


	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, t.width, t.height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, t.width, t.height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// create and attach depth buffer (renderbuffer)
	
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t.width, t.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// lighting info
	// -------------
	const unsigned int NR_LIGHTS = 32;

	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	// shader configuration
	// --------------------
	m_shaderLightingPass->Use();
	m_shaderLightingPass->SetInt("gPosition", 0);
	m_shaderLightingPass->SetInt("gNormal", 1);
	m_shaderLightingPass->SetInt("gAlbedoSpec", 2);

	RegisterOpaque(this);
}
void DeferredShading::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	glm::mat4 model;
	m_shaderGeometryPass->Use();
	m_shaderGeometryPass->SetMat4("projection", projection);
	m_shaderGeometryPass->SetMat4("view", view);
	for (unsigned int i = 0; i < objectPositions.size(); i++)
	{
		model = glm::mat4();
		model = glm::translate(model, objectPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		m_shaderGeometryPass->SetMat4("model", model);
		nanosuit->DrawMesh(m_shaderGeometryPass.get());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
	// -----------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shaderLightingPass->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	// send light relevant uniforms
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		m_shaderLightingPass->SetVec3("lights[" + KhSTL::ToString(i) + "].Position", lightPositions[i]);
		m_shaderLightingPass->SetVec3("lights[" + KhSTL::ToString(i) + "].Color", lightColors[i]);
		// update attenuation parameters and calculate radius
		const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const float linear = 0.7f;
		const float quadratic = 1.8f;
		m_shaderLightingPass->SetFloat("lights[" + KhSTL::ToString(i) + "].Linear", linear);
		m_shaderLightingPass->SetFloat("lights[" + KhSTL::ToString(i) + "].Quadratic", quadratic);
		// then calculate radius of light volume/sphere
		const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
		m_shaderLightingPass->SetFloat("lights[" + KhSTL::ToString(i) + "].Radius", radius);
	}
	m_shaderLightingPass->SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	// finally render quad
	renderQuad(&quadVAO,&quadVBO);

	// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
	// ----------------------------------------------------------------------------------
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
											   // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
											   // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
											   // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();
	glBlitFramebuffer(0, 0, t.width, t.height, 0, 0, t.width, t.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. render lights on top of scene
	// --------------------------------
	m_shaderLightBox->Use();
	m_shaderLightBox->SetMat4("projection", projection);
	m_shaderLightBox->SetMat4("view", view);
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		model = glm::mat4();
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.125f));
		m_shaderLightBox->SetMat4("model", model);
		m_shaderLightBox->SetVec3("lightColor", lightColors[i]);
		renderCube(&cubeVAO,&cubeVBO);
	}
}
}