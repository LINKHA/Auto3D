#pragma once
#include "Debug/Log.h"
#include "Resource/ResourceCache.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Engine/Components/Image.h"
#include "Renderer/Renderer.h"
#include "Graphics/Graphics.h"
#include "Core/Modules/ModuleManager.h"
#include "Scene/Scene.h"
#include "Platform/Context.h"
#include "Graphics/Texture.h"
#include "Engine/Components/Material.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Engine/Components/SkyBox.h"
#include "Graphics/Shader.h"

#include <Windows.h>

#include "Camera.h"
#include "DynamicModel.h"

#include <glad.h>

#include "Debug/DebugNew.h"


#include "DyDlobal.h"
#include "DyBackground.h"
#include "DyWaterTexture.h"


namespace Auto3D
{
static GLfloat g_projectionMatrix[16];

static GLfloat g_viewMatrix[16];

static GLfloat g_inverseViewNormalMatrix[9];

//

static GLuint g_vao;

static GLuint g_verticesVBO;

static GLuint g_indicesVBO;
//
static GLuint g_cubemap;

ADynamicModel::ADynamicModel()
{
	//GModuleManager::Get().RendererModule()->SetupWaterTextures(1, 1024, EImageFormat::RGB32F);
}

ADynamicModel::~ADynamicModel()
{
}

void ADynamicModel::RegisterObject()
{
	RegisterFactory<ADynamicModel>();
}
/*
void ADynamicModel::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
	_lastFrameNumber = frameNumber;
	_lightList = nullptr;
	_distance = camera->Distance(GetWorldPosition());

	//Draw water texture

}*/

//void ADynamicModel::SetWaterData()
//{
//	static float* waterPoints = (float*)malloc(WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(float));
//	static unsigned int* waterIndices = (unsigned int*)malloc(WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(unsigned int));
//	static bool waterPointsDirty = false;
//	static bool waterIndeicesDirty = false;
//
//	if (!waterPointsDirty)
//	{
//		waterPointsDirty = true;
//		for (int i = 0; i < WATER_PLANE_LENGTH; i++)
//		{
//			for (int k = 0; k < WATER_PLANE_LENGTH; k++)
//			{
//				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 0] = -(float)WATER_PLANE_LENGTH / 2 + 0.5f + (float)k;
//				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 1] = 0.0f;
//				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 2] = +(float)WATER_PLANE_LENGTH / 2 - 0.5f - (float)i;
//				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 3] = 1.0f;
//			}
//		}
//	}
//	
//	if (!waterIndeicesDirty)
//	{
//		waterIndeicesDirty = true;
//		for (int i = 0; i < WATER_PLANE_LENGTH - 1; i++)
//		{
//			for (int k = 0; k < WATER_PLANE_LENGTH; k++)
//			{
//				if (i % 2 == 0)
//				{
//					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 0] = k + (i + 1) * WATER_PLANE_LENGTH;
//					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 1] = k + i * WATER_PLANE_LENGTH;
//				}
//				else
//				{
//					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 0] = WATER_PLANE_LENGTH - 1 - k + i * WATER_PLANE_LENGTH;
//					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 1] = WATER_PLANE_LENGTH - 1 - k + (i + 1) * WATER_PLANE_LENGTH;
//				}
//			}
//		}
//	}
//
//
//
//	SetNumGeometries(1);
//
//	TVector<FVertexElement> vertexDeclaration;
//	vertexDeclaration.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::POSITION));//vertex
//
//	TSharedPtr<FVertexBuffer> vertexBuffer;
//	vertexBuffer->Define(EResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, waterPoints);
//
//
//	TSharedPtr<FIndexBuffer> indexBuffer;
//	indexBuffer->Define(EResourceUsage::IMMUTABLE, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH, sizeof(unsigned short), true, waterIndices);
//
//
//	TSharedPtr<FGeometry> geometry(new FGeometry());
//	// Temp use this setting
//	geometry->_lodDistance = false;
//	geometry->_primitiveType = EPrimitiveType::TRIANGLE_LIST;
//	geometry->_drawStart = 0;
//	geometry->_drawCount = WATER_PLANE_LENGTH * WATER_PLANE_LENGTH;
//
//	geometry->_vertexBuffer = vertexBuffer;
//	geometry->_indexBuffer = indexBuffer;
//
//	SetGeometry(0, geometry);
//
//}

static FShaderVariation* waterVSV = nullptr;
static FShaderVariation* waterPSV = nullptr;

bool ADynamicModel::init()
{
	auto* graphics = GModuleManager::Get().GraphicsModule();
	//graphics->SetGraphicsDebug(EGraphicsDebugType::LINE);

	GLfloat* points = (GLfloat*)malloc(WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat));
	GLuint* indices = (GLuint*)malloc(WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint));

	GLuint x, z, i, k;

	GLuint waterTexture;

	for (z = 0; z < WATER_PLANE_LENGTH; z++)
	{
		for (x = 0; x < WATER_PLANE_LENGTH; x++)
		{
			points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 0] = -(GLfloat)WATER_PLANE_LENGTH / 2 + 0.5f + (GLfloat)x;
			points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 1] = 0.0f;
			points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 2] = +(GLfloat)WATER_PLANE_LENGTH / 2 - 0.5f - (GLfloat)z;
			points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 3] = 1.0f;
		}
	}

	for (k = 0; k < WATER_PLANE_LENGTH - 1; k++)
	{
		for (i = 0; i < WATER_PLANE_LENGTH; i++)
		{
			if (k % 2 == 0)
			{
				indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 0] = i + (k + 1) * WATER_PLANE_LENGTH;
				indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 1] = i + k * WATER_PLANE_LENGTH;
			}
			else
			{
				indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 0] = WATER_PLANE_LENGTH - 1 - i + k * WATER_PLANE_LENGTH;
				indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 1] = WATER_PLANE_LENGTH - 1 - i + (k + 1) * WATER_PLANE_LENGTH;
			}
		}
	}

	auto cache = GModuleManager::Get().CacheModule();
	waterVSV = cache->LoadResource<AShader>("Shader/Water/Water.vert")->CreateVariation();
	waterPSV = cache->LoadResource<AShader>("Shader/Water/Water.frag")->CreateVariation();


	glGenBuffers(1, &g_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat), (GLfloat*)points, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint), (GLuint*)indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	free(points);
	free(indices);


	ATexture* cubeMap = GModuleManager::Get().RegisteredBoxModule()->GetActiveScene()->GetSkyBox()->GetMaterial(0)->GetTexture(0);
	g_cubemap = cubeMap->GetGLTexture();
	//

	waterTexture = initWaterTexture((GLUSfloat)WATER_PLANE_LENGTH);

	graphics->SetShaders(waterVSV, waterPSV);
	FShaderProgram* waterProgram = graphics->Shaderprogram();

	waterProgram->SetFloat("u_waterPlaneLength",(float)WATER_PLANE_LENGTH);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);
	waterProgram->SetInt("u_cubemap", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	waterProgram->SetInt("u_waterTexture", 1);

	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
	glVertexAttribPointer(waterProgram->GetAttribLocation("a_vertex"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(waterProgram->GetAttribLocation("a_vertex"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);



	glBindVertexArray(0);
	//initBackground();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return GLUS_TRUE;
}

void ADynamicModel::reshape(unsigned width,unsigned height)
{
	glViewport(0, 0, width, height);

	glusMatrix4x4Perspectivef(g_projectionMatrix, 40.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

	//reshapeBackground(g_projectionMatrix);

	reshapeWaterTexture(width, height);

	auto graphics = GModuleManager::Get().GraphicsModule();

	graphics->SetShaders(waterVSV, waterPSV);
	FShaderProgram* waterProgram = graphics->Shaderprogram();
	waterProgram->SetMat4("u_projectionMatrix", g_projectionMatrix);
}

void ADynamicModel::renderWater(float passedTime)
{
	static WaveParameters waveParameters[NUMBERWAVES];
	static WaveDirections waveDirections[NUMBERWAVES];

	static GLfloat overallSteepness = 0.2f;

	memset(waveParameters, 0, sizeof(waveParameters));
	memset(waveDirections, 0, sizeof(waveDirections));

	// Waves can be faded in and out.

	// Wave One
	waveParameters[0].speed = 1.0f;
	waveParameters[0].amplitude = 0.01f;
	waveParameters[0].wavelength = 4.0f;
	waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[0].x = +1.0f;
	waveDirections[0].z = +1.0f;

	// Wave Two
	waveParameters[1].speed = 0.5f;
	waveParameters[1].amplitude = 0.02f;
	waveParameters[1].wavelength = 3.0f;
	waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[1].x = +1.0f;
	waveDirections[1].z = +0.0f;

	// Wave Three
	waveParameters[2].speed = 0.1f;
	waveParameters[2].amplitude = 0.015f;
	waveParameters[2].wavelength = 2.0f;
	waveParameters[2].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[2].x = -0.1f;
	waveDirections[2].z = -0.2f;

	// Wave Four
	waveParameters[3].speed = 1.1f;
	waveParameters[3].amplitude = 0.008f;
	waveParameters[3].wavelength = 1.0f;
	waveParameters[3].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[3].x = -0.2f;
	waveDirections[3].z = -0.1f;

	auto graphics = GModuleManager::Get().GraphicsModule();

	graphics->SetShaders(waterVSV, waterPSV);
	FShaderProgram* waterProgram = graphics->Shaderprogram();

	waterProgram->SetMat4("u_viewMatrix", g_viewMatrix);
	waterProgram->SetMat3("u_inverseViewNormalMatrix", g_inverseViewNormalMatrix);

	waterProgram->SetFloat("u_passedTime", passedTime);
	waterProgram->SetVec4s("u_waveParameters", 4 * NUMBERWAVES, (float*)waveParameters);
	waterProgram->SetVec2s("u_waveDirections", 2 * NUMBERWAVES, (float*)waveDirections);
	glBindVertexArray(g_vao);

	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLE_STRIP, WATER_PLANE_LENGTH* (WATER_PLANE_LENGTH - 1) * 2, GL_UNSIGNED_INT, 0);
}




bool ADynamicModel::update(float time)
{
	time = 0.01f;


	static GLfloat passedTime = 0.0f;

	static GLUSfloat angle = 0.0f;

	GLfloat inverseViewMatrix[16];

	glusMatrix4x4LookAtf(g_viewMatrix, 0.0f, 1.0f, 0.0f, (GLfloat)0.5f * sinf(angle), 1.0f, -(GLfloat)0.5f * cosf(angle), 0.0f, 1.0f, 0.0f);

	glusMatrix4x4Copyf(inverseViewMatrix, g_viewMatrix, true);
	glusMatrix4x4InverseRigidBodyf(inverseViewMatrix);
	glusMatrix4x4ExtractMatrix3x3f(g_inverseViewNormalMatrix, inverseViewMatrix);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the background
	//renderBackground(g_viewMatrix);

	// Render the water texture
	renderWaterTexture(passedTime);

	// Render the water scene
	renderWater(passedTime);

	passedTime += time;

	angle += 2.0f * GLUS_PI / 120.0f * time;



	return true;
}


void ADynamicModel::AAA()
{
	auto window = GModuleManager::Get().GraphicsModule()->RenderWindow();
	init();
	reshape(window->GetWidth(), window->GetHeight());
}

void ADynamicModel::BBB()
{
	Sleep(10);
	update(0);
	//graphics->RenderContext()->Present();
}

}
