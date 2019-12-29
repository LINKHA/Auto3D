#include "SkyBox.h"
#include "Engine/Components/Image.h"
#include "Graphics/Texture.h"
#include "Resource/ResourceCache.h"
#include "Core/Modules/ModuleManager.h"
#include "Scene/WorldContext.h"
#include "Scene/World.h"
#include "Graphics/Graphics.h"
#include "Graphics/Shader.h"
#include "Engine/Components/Material.h"

#include "Model.h"

#include <glad.h>

#include "Debug/DebugNew.h"

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

static TMatrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
static TMatrix4x4F captureViews[] = {
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(1.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f, 0.0f)),
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(-1.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f, 0.0f)),
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, 1.0f, 0.0f), TVector3F(0.0f, 0.0f, 1.0f)),
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f, 0.0f), TVector3F(0.0f, 0.0f, -1.0f)),
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, 0.0f, 1.0f), TVector3F(0.0f, -1.0f, 0.0f)),
	LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, 0.0f, -1.0f), TVector3F(0.0f, -1.0f, 0.0f))
};

static FShaderVariation* irradianceVSV = nullptr;
static FShaderVariation* irradiancePSV = nullptr;

static FShaderVariation* prefilterVSV = nullptr;
static FShaderVariation* prefilterPSV = nullptr;

static FShaderVariation* brdfVSV = nullptr;
static FShaderVariation* brdfPSV = nullptr;

static bool isDirty = false;


REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ASkyBox)
	.constructor<>()
	.property("mapSize", &ASkyBox::GetMapSize, &ASkyBox::SetMapSize)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("irradianceSize", &ASkyBox::GetIrradianceSize, &ASkyBox::SetIrradianceSize)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("prefilterSize", &ASkyBox::GetPrefilterSize, &ASkyBox::SetPrefilterSize)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

ASkyBox::ASkyBox():
	_mapSize(512),
	_irradianceSize(32),
	_prefilterSize(128),
	_irradianceMap(nullptr),
	_prefilterMap(nullptr),
	_brdfLUT(nullptr)
{
	auto cache = GModuleManager::Get().CacheModule();
	SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	OnWorldBoundingBoxUpdate();
	GWorldContext::Get().GetActiveWorld()->SetSkyBox(this);
}

ASkyBox::~ASkyBox() = default;


void ASkyBox::SetupIBLMap()
{
	if (!isDirty)
	{
		auto cache = GModuleManager::Get().CacheModule();

		isDirty = true;

		irradianceVSV = cache->LoadResource<AShader>("Shader/IBL/Cubemap.vert")->CreateVariation();
		irradiancePSV = cache->LoadResource<AShader>("Shader/IBL/IrradianceConvolution.frag")->CreateVariation();

		prefilterVSV = cache->LoadResource<AShader>("Shader/IBL/Cubemap.vert")->CreateVariation();
		prefilterPSV = cache->LoadResource<AShader>("Shader/IBL/Prefilter.frag")->CreateVariation();

		brdfVSV = cache->LoadResource<AShader>("Shader/IBL/Brdf.vert")->CreateVariation();
		brdfPSV = cache->LoadResource<AShader>("Shader/IBL/Brdf.frag")->CreateVariation();
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

const TSharedPtr<ATexture>& ASkyBox::SetupIrradianceMap()
{
	if (_irradianceMap)
		_irradianceMap.Reset();

	auto graphics = GModuleManager::Get().GraphicsModule();

	_irradianceMap = TSharedPtr<ATexture>(new ATexture());
	_irradianceMap->Define(ETextureType::TEX_CUBE, EResourceUsage::DEFAULT, TVector2I(_irradianceSize, _irradianceSize), EImageFormat::RGBA16F, 1);
	_irradianceMap->DefineSampler(ETextureFilterMode::COMPARE_TRILINEAR, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
	_irradianceMap->SetDataLost(false);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _irradianceSize, _irradianceSize);

	graphics->SetShaders(irradianceVSV, irradiancePSV);
	FShaderProgram* irradianceProgram = graphics->Shaderprogram();

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

const TSharedPtr<ATexture>& ASkyBox::SetupPrefilterMap()
{
	if (_prefilterMap)
		_prefilterMap.Reset();

	_prefilterMap = TSharedPtr<ATexture>(new ATexture());
	_prefilterMap->Define(ETextureType::TEX_CUBE, EResourceUsage::DEFAULT, TVector2I(_prefilterSize, _prefilterSize), EImageFormat::RGBA16F, 1);
	_prefilterMap->DefineSampler(ETextureFilterMode::COMPARE_TRILINEAR, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
	_prefilterMap->SetDataLost(false);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	auto graphics = GModuleManager::Get().GraphicsModule();
	graphics->SetShaders(prefilterVSV, prefilterPSV);
	FShaderProgram* prefilterProgram = graphics->Shaderprogram();

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

const TSharedPtr<ATexture>& ASkyBox::SetupBrdfLUT()
{
	if (_brdfLUT)
		_brdfLUT.Reset();

	_brdfLUT = TSharedPtr<ATexture>(new ATexture());
	_brdfLUT->Define(ETextureType::TEX_2D, EResourceUsage::DEFAULT, TVector2I(_mapSize, _mapSize), EImageFormat::RG16F, 1);
	_brdfLUT->DefineSampler(ETextureFilterMode::COMPARE_TRILINEAR, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
	_brdfLUT->SetDataLost(false);

	// Re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _mapSize, _mapSize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _brdfLUT->GetGLTexture(), 0);

	glViewport(0, 0, _mapSize, _mapSize);
	auto graphics = GModuleManager::Get().GraphicsModule();
	graphics->SetShaders(brdfVSV, brdfPSV);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderQuad();

	return _brdfLUT;
}

const TSharedPtr<ATexture>& ASkyBox::GetIrradianceMap()
{
	return _irradianceMap;
}

const TSharedPtr<ATexture>& ASkyBox::GetPrefilterMap()
{
	return _prefilterMap;
}

const TSharedPtr<ATexture>& ASkyBox::GetBrdfLUT()
{
	return _brdfLUT;
}

void ASkyBox::OnWorldBoundingBoxUpdate()
{
	// The skybox is supposed to be visible everywhere, so set a humongous bounding box
	_boundingBox.Define(-M_FLOAT_BIG, M_FLOAT_BIG);
}

}