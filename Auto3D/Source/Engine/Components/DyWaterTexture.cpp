#include "DyWaterTexture.h"

namespace Auto3D
{
static GLuint g_vaoWaterTexture;

static GLuint g_verticesWaterTextureVBO;

static GLuint g_texCoordsWaterTextureVBO;

static GLuint g_indicesWaterTextureVBO;

static GLuint g_numberIndicesWaterTexture;

//

static GLuint g_mirrorTexture;

static GLuint g_depthMirrorTexture;

//

static GLuint g_fboWaterTexture;

//

/**
	* Width of the parent/caller element.
	*/
static GLuint g_parentWidth;

/**
	* Height of the parent/caller element.
	*/
static GLuint g_parentHeight;


static FShaderVariation* waterTextureVSV = nullptr;
static FShaderVariation* waterTexturePSV = nullptr;


static bool isDirty = false;

GLUSvoid reshapeWaterTexture(GLUSint width, GLUSint height)
{
	// Store parent/caller width ..
	g_parentWidth = width;

	// ... and height for later usage to set to the original values
	g_parentHeight = height;
}


GLUSuint initWaterTexture(GLUSfloat waterPlaneLength)
{

	GLUSshape plane;

	auto cache = GModuleManager::Get().CacheModule();

	waterTextureVSV = cache->LoadResource<AShader>("Shader/Water/WaterTexture.vert")->CreateVariation();
	waterTexturePSV = cache->LoadResource<AShader>("Shader/Water/WaterTexture.frag")->CreateVariation();

	glGenTextures(1, &g_mirrorTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_mirrorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GLUS_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GLUS_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	//

	glGenRenderbuffers(1, &g_depthMirrorTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, g_depthMirrorTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_SIZE, TEXTURE_SIZE);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//

	glGenFramebuffers(1, &g_fboWaterTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fboWaterTexture);


	// Attach the color buffer ...
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_mirrorTexture, 0);

	// ... and the depth buffer,
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depthMirrorTexture);



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

		return GLUS_FALSE;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//

	glBindVertexArray(0);

	//

	glusShapeCreatePlanef(&plane, TEXTURE_SIZE / 2.0f);
	g_numberIndicesWaterTexture = plane.numberIndices;

	glGenBuffers(1, &g_verticesWaterTextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesWaterTextureVBO);
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 4 * sizeof(GLfloat), (GLfloat*)plane.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &g_texCoordsWaterTextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsWaterTextureVBO);
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 2 * sizeof(GLfloat), (GLfloat*)plane.texCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indicesWaterTextureVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesWaterTextureVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.numberIndices * sizeof(GLuint), (GLuint*)plane.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glusShapeDestroyf(&plane);

	GLfloat projectionMatrixWaterTexture[16];
	GLfloat modelViewMatrixWaterTexture[16];


	auto graphics = GModuleManager::Get().GraphicsModule();
	graphics->SetShaders(waterTextureVSV, waterTexturePSV);
	FShaderProgram* waterTextureProgram = graphics->Shaderprogram();

	glusMatrix4x4LookAtf(modelViewMatrixWaterTexture, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	waterTextureProgram->SetMat4("u_modelViewMatrix", modelViewMatrixWaterTexture);

	glusMatrix4x4Orthof(projectionMatrixWaterTexture, -(GLfloat)TEXTURE_SIZE / 2, (GLfloat)TEXTURE_SIZE / 2, -(GLfloat)TEXTURE_SIZE / 2, (GLfloat)TEXTURE_SIZE / 2, 1.0f, 100.0f);

	waterTextureProgram->SetMat4("u_projectionMatrix", projectionMatrixWaterTexture);
	waterTextureProgram->SetFloat("u_waterPlaneLength", WATER_PLANE_LENGTH);

	glGenVertexArrays(1, &g_vaoWaterTexture);
	glBindVertexArray(g_vaoWaterTexture);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesWaterTextureVBO);

	glVertexAttribPointer(glGetAttribLocation(waterTextureProgram->GLProgram(), "a_vertex"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(waterTextureProgram->GLProgram(), "a_vertex"));

	glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsWaterTextureVBO);

	glVertexAttribPointer(glGetAttribLocation(waterTextureProgram->GLProgram(), "a_texCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(waterTextureProgram->GLProgram(), "a_texCoord"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesWaterTextureVBO);


	//

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return g_mirrorTexture;
}


GLUSboolean renderWaterTexture(GLUSfloat passedTime)
{
		

	static WaveParameters waveParameters[NUMBERWAVES];
	static WaveDirections waveDirections[NUMBERWAVES];

	static GLfloat overallSteepness = 0.2f;

	// Waves

	memset(waveParameters, 0, sizeof(waveParameters));
	memset(waveDirections, 0, sizeof(waveDirections));

	// Wave One
	waveParameters[0].speed = 0.05f;
	waveParameters[0].amplitude = 0.02f;
	waveParameters[0].wavelength = 0.3f;
	waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[0].x = +1.0f;
	waveDirections[0].z = +1.5f;

	// Wave Two
	waveParameters[1].speed = 0.1f;
	waveParameters[1].amplitude = 0.01f;
	waveParameters[1].wavelength = 0.4f;
	waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[1].x = +0.8f;
	waveDirections[1].z = +0.2f;

	// Wave Thre
	waveParameters[2].speed = 0.04f;
	waveParameters[2].amplitude = 0.035f;
	waveParameters[2].wavelength = 0.1f;
	waveParameters[2].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[2].x = -0.2f;
	waveDirections[2].z = -0.1f;

	// Wave Four
	waveParameters[3].speed = 0.05f;
	waveParameters[3].amplitude = 0.007f;
	waveParameters[3].wavelength = 0.2f;
	waveParameters[3].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat)NUMBERWAVES);
	waveDirections[3].x = -0.4f;
	waveDirections[3].z = -0.3f;

	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	glBindFramebuffer(GL_FRAMEBUFFER, g_fboWaterTexture);

	//

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto graphics = GModuleManager::Get().GraphicsModule();
	graphics->SetShaders(waterTextureVSV, waterTexturePSV);
	FShaderProgram* waterTextureProgram = graphics->Shaderprogram();

	waterTextureProgram->SetFloat("u_passedTime", passedTime);
	waterTextureProgram->SetVec4s("u_waveParameters", 4 * NUMBERWAVES, (float*)waveParameters);
	waterTextureProgram->SetVec2s("u_waveDirections", 2 * NUMBERWAVES, (float*)waveDirections);

	glFrontFace(GL_CCW);

	glBindVertexArray(g_vaoWaterTexture);
	glDrawElements(GL_TRIANGLES, g_numberIndicesWaterTexture, GL_UNSIGNED_INT, 0);

	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, g_parentWidth, g_parentHeight);

	return GLUS_TRUE;
}

}