#include "DyBackground.h"

namespace Auto3D
{

static GLuint g_vaoBackground;

static GLuint g_verticesBackgroundVBO;

static GLuint g_normalsBackgroundVBO;

static GLuint g_indicesBackgroundVBO;

static GLuint g_numberIndicesBackground;

static ShaderVariation* backGroundVSV = nullptr;
static ShaderVariation* backGroundPSV = nullptr;

GLUSboolean initBackground()
{
	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;

	GLUSshape background;

	auto cache = GModuleManager::Get().CacheModule();
	backGroundVSV = cache->LoadResource<Shader>("Shader/Water/Background.vert")->CreateVariation();
	backGroundPSV = cache->LoadResource<Shader>("Shader/Water/Background.frag")->CreateVariation();

	glBindVertexArray(0);

	//

	glusShapeCreateSpheref(&background, (GLfloat)(GLfloat)WATER_PLANE_LENGTH / 2.0f + 0.5f, 32);
	g_numberIndicesBackground = background.numberIndices;

	glGenBuffers(1, &g_verticesBackgroundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 4 * sizeof(GLfloat), (GLfloat*)background.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &g_normalsBackgroundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 3 * sizeof(GLfloat), (GLfloat*)background.normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indicesBackgroundVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, background.numberIndices * sizeof(GLuint), (GLuint*)background.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glusShapeDestroyf(&background);


	auto graphics = GModuleManager::Get().GraphicsModule();

	graphics->SetShaders(backGroundVSV, backGroundPSV);
	ShaderProgram* waterProgram = graphics->Shaderprogram();

	waterProgram->SetInt("u_cubemap", 0);

	glGenVertexArrays(1, &g_vaoBackground);
	glBindVertexArray(g_vaoBackground);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
	glVertexAttribPointer(waterProgram->GetAttribLocation("a_vertex"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(waterProgram->GetAttribLocation("a_vertex"));

	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
	glVertexAttribPointer(waterProgram->GetAttribLocation("a_normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(waterProgram->GetAttribLocation("a_normal"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);


	return GLUS_TRUE;
}



GLUSvoid reshapeBackground(GLUSfloat projectionMatrix[16])
{

	auto graphics = GModuleManager::Get().GraphicsModule();

	graphics->SetShaders(backGroundVSV, backGroundPSV);
	ShaderProgram* waterProgram = graphics->Shaderprogram();

	waterProgram->SetMat4("u_projectionMatrix", projectionMatrix);
}

GLUSboolean renderBackground(GLUSfloat viewMatrix[16])
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto graphics = GModuleManager::Get().GraphicsModule();

	graphics->SetShaders(backGroundVSV, backGroundPSV);
	ShaderProgram* waterProgram = graphics->Shaderprogram();

	waterProgram->SetMat4("u_modelViewMatrix", viewMatrix);

	glBindVertexArray(g_vaoBackground);

	glFrontFace(GL_CW);

	glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

	return GLUS_TRUE;
}
}