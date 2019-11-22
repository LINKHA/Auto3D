#include "SkyBox.h"
#include "../Resource/Image.h"
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"
#include "../Engine/ModuleManager.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Shader.h"
#include "../Renderer/Material.h"

#include "Model.h"

#include <glad.h>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void RenderCube()
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	unsigned int cubeVBO = 0;

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderQuad()
{

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	unsigned int quadVBO;
	// setup plane VAO
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static Matrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
static Matrix4x4F captureViews[] = {
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(1.0f, 0.0f, 0.0f), Vector3F(0.0f, -1.0f, 0.0f)),
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(-1.0f, 0.0f, 0.0f), Vector3F(0.0f, -1.0f, 0.0f)),
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, 1.0f, 0.0f), Vector3F(0.0f, 0.0f, 1.0f)),
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, -1.0f, 0.0f), Vector3F(0.0f, 0.0f, -1.0f)),
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, 0.0f, 1.0f), Vector3F(0.0f, -1.0f, 0.0f)),
	LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, 0.0f, -1.0f), Vector3F(0.0f, -1.0f, 0.0f))
};

static ShaderVariation* irradianceVSV = nullptr;
static ShaderVariation* irradiancePSV = nullptr;

static ShaderVariation* prefilterVSV = nullptr;
static ShaderVariation* prefilterPSV = nullptr;

static ShaderVariation* brdfVSV = nullptr;
static ShaderVariation* brdfPSV = nullptr;

static bool isDirty = false;
SkyBox::SkyBox():
	_mapSize(512),
	_irradianceSize(32),
	_prefilterSize(128),
	_irradianceMap(nullptr),
	_prefilterMap(nullptr),
	_brdfLUT(nullptr)
{
	auto cache = ModuleManager::Get().CacheModule();
	SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
	OnWorldBoundingBoxUpdate();
	ModuleManager::Get().RegisteredBoxModule()->GetActiveScene()->SetSkyBox(this);
}

SkyBox::~SkyBox() = default;


void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
}

void SkyBox::SetupIBLMap()
{
	if (!isDirty)
	{
		auto cache = ModuleManager::Get().CacheModule();

		isDirty = true;

		irradianceVSV = cache->LoadResource<Shader>("Shader/IBL/Cubemap.vert")->CreateVariation();
		irradiancePSV = cache->LoadResource<Shader>("Shader/IBL/IrradianceConvolution.frag")->CreateVariation();

		prefilterVSV = cache->LoadResource<Shader>("Shader/IBL/Cubemap.vert")->CreateVariation();
		prefilterPSV = cache->LoadResource<Shader>("Shader/IBL/Prefilter.frag")->CreateVariation();

		brdfVSV = cache->LoadResource<Shader>("Shader/IBL/Brdf.vert")->CreateVariation();
		brdfPSV = cache->LoadResource<Shader>("Shader/IBL/Brdf.frag")->CreateVariation();
	}


	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _mapSize, _mapSize);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	SetupIrradianceMap();
	SetupPrefilterMap();
	SetupBrdfLUT();

	glDeleteRenderbuffers(1, &captureRBO);
	glDeleteFramebuffers(1, &captureFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const SharedPtr<Texture>& SkyBox::SetupIrradianceMap()
{
	if (_irradianceMap)
		_irradianceMap.Reset();

	auto graphics = ModuleManager::Get().GraphicsModule();

	_irradianceMap = SharedPtr<Texture>(new Texture());
	_irradianceMap->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(_irradianceSize, _irradianceSize), ImageFormat::RGBA16F, 1);
	_irradianceMap->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	_irradianceMap->SetDataLost(false);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _irradianceSize, _irradianceSize);

	graphics->SetShaders(irradianceVSV, irradiancePSV);
	ShaderProgram* irradianceProgram = graphics->Shaderprogram();

	irradianceProgram->SetInt("environmentMap", 0);
	irradianceProgram->SetMat4("projection", captureProjection);
	graphics->SetTexture(0, GetMaterial(0)->_textures[0]);

	glViewport(0, 0, _irradianceSize, _irradianceSize);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceProgram->SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _irradianceMap->GetGLTexture(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}

	return _irradianceMap;
}

const SharedPtr<Texture>& SkyBox::SetupPrefilterMap()
{
	if (_prefilterMap)
		_prefilterMap.Reset();

	_prefilterMap = SharedPtr<Texture>(new Texture());
	_prefilterMap->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(_prefilterSize, _prefilterSize), ImageFormat::RGBA16F, 1);
	_prefilterMap->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	_prefilterMap->SetDataLost(false);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	auto graphics = ModuleManager::Get().GraphicsModule();
	graphics->SetShaders(prefilterVSV, prefilterPSV);
	ShaderProgram* prefilterProgram = graphics->Shaderprogram();

	prefilterProgram->SetInt("environmentMap", 0);
	prefilterProgram->SetMat4("projection", captureProjection);
	graphics->SetTexture(0, GetMaterial(0)->_textures[0]);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// Reisze framebuffer according to mip-level size.
		unsigned int mipWidth = _prefilterSize * Pow(0.5, mip);
		unsigned int mipHeight = _prefilterSize * Pow(0.5, mip);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterProgram->SetFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterProgram->SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _prefilterMap->GetGLTexture(), mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCube();
		}
	}

	return _prefilterMap;
}

const SharedPtr<Texture>& SkyBox::SetupBrdfLUT()
{
	if (_brdfLUT)
		_brdfLUT.Reset();

	_brdfLUT = SharedPtr<Texture>(new Texture());
	_brdfLUT->Define(TextureType::TEX_2D, ResourceUsage::DEFAULT, Vector2I(_mapSize, _mapSize), ImageFormat::RG16F, 1);
	_brdfLUT->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	_brdfLUT->SetDataLost(false);

	// Re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _mapSize, _mapSize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _brdfLUT->GetGLTexture(), 0);

	glViewport(0, 0, _mapSize, _mapSize);
	auto graphics = ModuleManager::Get().GraphicsModule();
	graphics->SetShaders(brdfVSV, brdfPSV);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderQuad();

	return _brdfLUT;
}

const SharedPtr<Texture>& SkyBox::GetIrradianceMap()
{
	return _irradianceMap;
}

const SharedPtr<Texture>& SkyBox::GetPrefilterMap()
{
	return _prefilterMap;
}

const SharedPtr<Texture>& SkyBox::GetBrdfLUT()
{
	return _brdfLUT;
}

void SkyBox::OnWorldBoundingBoxUpdate()
{
	// The skybox is supposed to be visible everywhere, so set a humongous bounding box
	_boundingBox.Define(-M_FLOAT_BIG, M_FLOAT_BIG);
}

}