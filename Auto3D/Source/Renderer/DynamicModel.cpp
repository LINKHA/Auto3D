#pragma once
#include "../Debug/Log.h"
#include "../Resource/ResourceCache.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Resource/Image.h"
#include "../Renderer/Renderer.h"
#include "../Graphics/Graphics.h"
#include "../Engine/ModuleManager.h"

#include "Camera.h"
#include "DynamicModel.h"

#include <glad.h>

#include "../Debug/DebugNew.h"


#include "DyDlobal.h"


namespace Auto3D
{
	static GLfloat g_projectionMatrix[16];

	static GLfloat g_viewMatrix[16];

	static GLfloat g_inverseViewNormalMatrix[9];

	//

	static GLUSprogram g_program;

	static GLint g_projectionMatrixLocation;

	static GLint g_viewMatrixLocation;

	static GLint g_inverseViewNormalMatrixLocation;

	static GLint g_waterPlaneLengthLocation;

	static GLint g_passedTimeLocation;

	static GLint g_waveParametersLocation;

	static GLint g_waveDirectionsLocation;

	static GLint g_vertexLocation;

	static GLint g_cubemapLocation;

	static GLint g_waterTextureLocation;

	//

	static GLuint g_vao;

	static GLuint g_verticesVBO;

	static GLuint g_indicesVBO;
	//
	static GLuint g_cubemap;





	static GLUSprogram g_programWaterTexture;

	static GLint g_projectionMatrixWaterTextureLocation;

	static GLint g_modelViewMatrixWaterTextureLocation;

	static GLint g_vertexWaterTextureLocation;

	static GLint g_texCoordWaterTextureLocation;

	static GLint g_waterPlaneLengthWaterTextureLocation;

	static GLint g_passedTimeWaterTextureLocation;

	static GLint g_waveParametersWaterTextureLocation;

	static GLint g_waveDirectionsWaterTextureLocation;

	//

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


GLUSuint initWaterTexture(GLUSfloat waterPlaneLength)
{
	GLfloat projectionMatrixWaterTexture[16];
	GLfloat modelViewMatrixWaterTexture[16];

	GLUSshape plane;

	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;

	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/WaterTexture.vert.glsl", &vertexSource);
	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/WaterTexture.frag.glsl", &fragmentSource);

	glusProgramBuildFromSource(&g_programWaterTexture, (const GLUSchar**)&vertexSource.text, 0, 0, 0, (const GLUSchar**)&fragmentSource.text);

	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);

	//

	g_projectionMatrixWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_projectionMatrix");
	g_modelViewMatrixWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_modelViewMatrix");

	g_waterPlaneLengthWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_waterPlaneLength");
	g_passedTimeWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_passedTime");
	g_waveParametersWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_waveParameters");
	g_waveDirectionsWaterTextureLocation = glGetUniformLocation(g_programWaterTexture.program, "u_waveDirections");

	g_vertexWaterTextureLocation = glGetAttribLocation(g_programWaterTexture.program, "a_vertex");
	g_texCoordWaterTextureLocation = glGetAttribLocation(g_programWaterTexture.program, "a_texCoord");

	//

	glGenTextures(1, &g_mirrorTexture);
	glActiveTexture(GL_TEXTURE1);
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

	//

	glUseProgram(g_programWaterTexture.program);

	glusMatrix4x4LookAtf(modelViewMatrixWaterTexture, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(g_modelViewMatrixWaterTextureLocation, 1, GL_FALSE, modelViewMatrixWaterTexture);

	glusMatrix4x4Orthof(projectionMatrixWaterTexture, -(GLfloat)TEXTURE_SIZE / 2, (GLfloat)TEXTURE_SIZE / 2, -(GLfloat)TEXTURE_SIZE / 2, (GLfloat)TEXTURE_SIZE / 2, 1.0f, 100.0f);
	glUniformMatrix4fv(g_projectionMatrixWaterTextureLocation, 1, GL_FALSE, projectionMatrixWaterTexture);

	glUniform1f(g_waterPlaneLengthWaterTextureLocation, waterPlaneLength);

	//

	glGenVertexArrays(1, &g_vaoWaterTexture);
	glBindVertexArray(g_vaoWaterTexture);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesWaterTextureVBO);
	glVertexAttribPointer(g_vertexWaterTextureLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_vertexWaterTextureLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsWaterTextureVBO);
	glVertexAttribPointer(g_texCoordWaterTextureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_texCoordWaterTextureLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesWaterTextureVBO);
	//

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return g_mirrorTexture;
}


static GLUSprogram g_programBackground;

static GLint g_projectionMatrixBackgroundLocation;

static GLint g_modelViewMatrixBackgroundLocation;

static GLint g_vertexBackgroundLocation;

static GLint g_normalBackgroundLocation;

static GLint g_cubemapBackgroundLocation;

//

static GLuint g_vaoBackground;

static GLuint g_verticesBackgroundVBO;

static GLuint g_normalsBackgroundVBO;

static GLuint g_indicesBackgroundVBO;

static GLuint g_numberIndicesBackground;


GLUSboolean initBackground()
{
	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;

	GLUSshape background;

	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/Background.vert.glsl", &vertexSource);
	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/Background.frag.glsl", &fragmentSource);

	glusProgramBuildFromSource(&g_programBackground, (const GLUSchar**)&vertexSource.text, 0, 0, 0, (const GLUSchar**)&fragmentSource.text);

	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);

	//

	g_projectionMatrixBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_projectionMatrix");
	g_modelViewMatrixBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_modelViewMatrix");

	g_cubemapBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_cubemap");

	g_vertexBackgroundLocation = glGetAttribLocation(g_programBackground.program, "a_vertex");
	g_normalBackgroundLocation = glGetAttribLocation(g_programBackground.program, "a_normal");

	//

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

	//

	glUseProgram(g_programBackground.program);

	// We assume, that the parent program created the texture!
	glUniform1i(g_cubemapBackgroundLocation, 0);

	//

	glGenVertexArrays(1, &g_vaoBackground);
	glBindVertexArray(g_vaoBackground);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
	glVertexAttribPointer(g_vertexBackgroundLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_vertexBackgroundLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
	glVertexAttribPointer(g_normalBackgroundLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_normalBackgroundLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);

	return GLUS_TRUE;
}


DynamicModel::DynamicModel()
{
	//ModuleManager::Get().RendererModule()->SetupWaterTextures(1, 1024, ImageFormat::RGB32F);
}

DynamicModel::~DynamicModel()
{
}

void DynamicModel::RegisterObject()
{
	RegisterFactory<DynamicModel>();
}

void DynamicModel::OnPrepareRender(unsigned frameNumber, Camera* camera)
{
	_lastFrameNumber = frameNumber;
	_lightList = nullptr;
	_distance = camera->Distance(GetWorldPosition());

	//Draw water texture

}

//void DynamicModel::SetWaterData()
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
//	Vector<VertexElement> vertexDeclaration;
//	vertexDeclaration.Push(VertexElement(ElementType::VECTOR4, ElementSemantic::POSITION));//vertex
//
//	SharedPtr<VertexBuffer> vertexBuffer;
//	vertexBuffer->Define(ResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, waterPoints);
//
//
//	SharedPtr<IndexBuffer> indexBuffer;
//	indexBuffer->Define(ResourceUsage::IMMUTABLE, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH, sizeof(unsigned short), true, waterIndices);
//
//
//	SharedPtr<Geometry> geometry(new Geometry());
//	// Temp use this setting
//	geometry->_lodDistance = false;
//	geometry->_primitiveType = PrimitiveType::TRIANGLE_LIST;
//	geometry->_drawStart = 0;
//	geometry->_drawCount = WATER_PLANE_LENGTH * WATER_PLANE_LENGTH;
//
//	geometry->_vertexBuffer = vertexBuffer;
//	geometry->_indexBuffer = indexBuffer;
//
//	SetGeometry(0, geometry);
//
//}

bool DynamicModel::init()
{

	/*glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

	GLfloat* points = (GLfloat*)malloc(WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat));
	GLuint* indices = (GLuint*)malloc(WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint));

	GLUStgaimage image;

	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;

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

	//

	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/Water.vert.glsl", &vertexSource);
	glusFileLoadText("E:/Project/MyProject/opengl_tutorial_demo/Example15/shader/Water.frag.glsl", &fragmentSource);

	glusProgramBuildFromSource(&g_program, (const GLUSchar**)&vertexSource.text, 0, 0, 0, (const GLUSchar**)&fragmentSource.text);

	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);

	//

	g_projectionMatrixLocation = glGetUniformLocation(g_program.program, "u_projectionMatrix");
	g_viewMatrixLocation = glGetUniformLocation(g_program.program, "u_viewMatrix");
	g_inverseViewNormalMatrixLocation = glGetUniformLocation(g_program.program, "u_inverseViewNormalMatrix");

	g_waterPlaneLengthLocation = glGetUniformLocation(g_program.program, "u_waterPlaneLength");

	g_cubemapLocation = glGetUniformLocation(g_program.program, "u_cubemap");

	g_waterTextureLocation = glGetUniformLocation(g_program.program, "u_waterTexture");

	g_passedTimeLocation = glGetUniformLocation(g_program.program, "u_passedTime");

	g_waveParametersLocation = glGetUniformLocation(g_program.program, "u_waveParameters");
	g_waveDirectionsLocation = glGetUniformLocation(g_program.program, "u_waveDirections");

	g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");

	//

	glGenBuffers(1, &g_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat), (GLfloat*)points, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint), (GLuint*)indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//

	free(points);
	free(indices);

	//

	glGenTextures(1, &g_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_pos_x.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_neg_x.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_pos_y.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_neg_y.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_pos_z.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glusImageLoadTga("E:/Project/MyProject/opengl_tutorial_demo/Binaries/water_neg_z.tga", &image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusImageDestroyTga(&image);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//

	waterTexture = initWaterTexture((GLUSfloat)WATER_PLANE_LENGTH);

	glUseProgram(g_program.program);

	glUniform1f(g_waterPlaneLengthLocation, (GLUSfloat)WATER_PLANE_LENGTH);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);
	glUniform1i(g_cubemapLocation, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glUniform1i(g_waterTextureLocation, 1);

	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
	glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_vertexLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

	//

	initBackground();

	//

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return GLUS_TRUE;
}

GLUSvoid reshapeBackground(GLUSfloat projectionMatrix[16])
{
	glUseProgram(g_programBackground.program);

	glUniformMatrix4fv(g_projectionMatrixBackgroundLocation, 1, GL_FALSE, projectionMatrix);
}

GLUSvoid reshapeWaterTexture(GLUSint width, GLUSint height)
{
	// Store parent/caller width ..
	g_parentWidth = width;

	// ... and height for later usage to set to the original values
	g_parentHeight = height;
}
void DynamicModel::reshape(unsigned width,unsigned height)
{
	glViewport(0, 0, width, height);

	glusMatrix4x4Perspectivef(g_projectionMatrix, 40.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

	reshapeBackground(g_projectionMatrix);

	reshapeWaterTexture(width, height);

	glUseProgram(g_program.program);

	glUniformMatrix4fv(g_projectionMatrixLocation, 1, GL_FALSE, g_projectionMatrix);
}
typedef struct _WaveParameters
{
	GLfloat speed;
	GLfloat amplitude;
	GLfloat wavelength;
	GLfloat steepness;
} WaveParameters;

typedef struct _WaveDirections
{
	GLfloat x;
	GLfloat z;
} WaveDirections;

void DynamicModel::renderWater(float passedTime)
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

	glUseProgram(g_program.program);

	glUniformMatrix4fv(g_viewMatrixLocation, 1, GL_FALSE, g_viewMatrix);
	glUniformMatrix3fv(g_inverseViewNormalMatrixLocation, 1, GL_FALSE, g_inverseViewNormalMatrix);

	glUniform1f(g_passedTimeLocation, passedTime);

	glUniform4fv(g_waveParametersLocation, 4 * NUMBERWAVES, (GLfloat*)waveParameters);
	glUniform2fv(g_waveDirectionsLocation, 2 * NUMBERWAVES, (GLfloat*)waveDirections);

	glBindVertexArray(g_vao);

	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLE_STRIP, WATER_PLANE_LENGTH* (WATER_PLANE_LENGTH - 1) * 2, GL_UNSIGNED_INT, 0);
}



GLUSboolean renderBackground(GLUSfloat viewMatrix[16])
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(g_programBackground.program);

	glUniformMatrix4fv(g_modelViewMatrixBackgroundLocation, 1, GL_FALSE, viewMatrix);

	glBindVertexArray(g_vaoBackground);

	glFrontFace(GL_CW);

	glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

	return GLUS_TRUE;
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

	glUseProgram(g_programWaterTexture.program);

	glUniform1f(g_passedTimeWaterTextureLocation, passedTime);
	glUniform4fv(g_waveParametersWaterTextureLocation, 4 * NUMBERWAVES, (GLfloat*)waveParameters);
	glUniform2fv(g_waveDirectionsWaterTextureLocation, 2 * NUMBERWAVES, (GLfloat*)waveDirections);

	glFrontFace(GL_CCW);

	glBindVertexArray(g_vaoWaterTexture);
	glDrawElements(GL_TRIANGLES, g_numberIndicesWaterTexture, GL_UNSIGNED_INT, 0);

	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, g_parentWidth, g_parentHeight);

	return GLUS_TRUE;
}


bool DynamicModel::update(float time)
{
	time = 0.01f;


	static GLfloat passedTime = 0.0f;

	static GLUSfloat angle = 0.0f;

	GLfloat inverseViewMatrix[16];

	glusMatrix4x4LookAtf(g_viewMatrix, 0.0f, 1.0f, 0.0f, (GLfloat)0.5f * sinf(angle), 1.0f, -(GLfloat)0.5f * cosf(angle), 0.0f, 1.0f, 0.0f);

	glusMatrix4x4Copyf(inverseViewMatrix, g_viewMatrix, GLUS_TRUE);
	glusMatrix4x4InverseRigidBodyf(inverseViewMatrix);
	glusMatrix4x4ExtractMatrix3x3f(g_inverseViewNormalMatrix, inverseViewMatrix);

	// Render the background
	renderBackground(g_viewMatrix);

	// Render the water texture
	renderWaterTexture(passedTime);

	// Render the water scene
	renderWater(passedTime);

	passedTime += time;

	angle += 2.0f * GLUS_PI / 120.0f * time;

	return GLUS_TRUE;
}


void DynamicModel::AAA()
{
	auto window = ModuleManager::Get().GraphicsModule()->RenderWindow();
	auto graphics = ModuleManager::Get().GraphicsModule();
	init();
	reshape(window->GetWidth(), window->GetHeight());
	while (1)
	{
		Sleep(10);
		update(0);
		graphics->Present();
	}
}

}
