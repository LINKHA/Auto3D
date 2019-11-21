#include "DyBackground.h"

namespace Auto3D
{
#define NoDebug2 0

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

	static ShaderVariation* backGroundVSV = nullptr;
	static ShaderVariation* backGroundPSV = nullptr;

	GLUSboolean initBackground()
	{
		GLUStextfile vertexSource;
		GLUStextfile fragmentSource;

		GLUSshape background;
#if NoDebug2
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
#else
		auto cache = ModuleManager::Get().CacheModule();
		backGroundVSV = cache->LoadResource<Shader>("Shader/Water/Background.vert")->CreateVariation();
		backGroundPSV = cache->LoadResource<Shader>("Shader/Water/Background.frag")->CreateVariation();
#endif
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
#if NoDebug2
		glUseProgram(g_programBackground.program);

		// We assume, that the parent program created the texture!
		glUniform1i(g_cubemapBackgroundLocation, 0);

		glGenVertexArrays(1, &g_vaoBackground);
		glBindVertexArray(g_vaoBackground);

		glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
		glVertexAttribPointer(g_vertexBackgroundLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(g_vertexBackgroundLocation);

		glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
		glVertexAttribPointer(g_normalBackgroundLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(g_normalBackgroundLocation);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);

#else
		auto graphics = ModuleManager::Get().GraphicsModule();

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
#endif
	

		return GLUS_TRUE;
	}



	GLUSvoid reshapeBackground(GLUSfloat projectionMatrix[16])
	{
#if NoDebug2
		glUseProgram(g_programBackground.program);

		glUniformMatrix4fv(g_projectionMatrixBackgroundLocation, 1, GL_FALSE, projectionMatrix);
#else
		auto graphics = ModuleManager::Get().GraphicsModule();

		graphics->SetShaders(backGroundVSV, backGroundPSV);
		ShaderProgram* waterProgram = graphics->Shaderprogram();

		waterProgram->SetMat4("u_projectionMatrix", projectionMatrix);
#endif
	}

	GLUSboolean renderBackground(GLUSfloat viewMatrix[16])
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if NoDebug2
		glUseProgram(g_programBackground.program);

		glUniformMatrix4fv(g_modelViewMatrixBackgroundLocation, 1, GL_FALSE, viewMatrix);
#else
		auto graphics = ModuleManager::Get().GraphicsModule();

		graphics->SetShaders(backGroundVSV, backGroundPSV);
		ShaderProgram* waterProgram = graphics->Shaderprogram();

		waterProgram->SetMat4("u_modelViewMatrix", viewMatrix);
#endif
		glBindVertexArray(g_vaoBackground);

		glFrontFace(GL_CW);

		glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

		return GLUS_TRUE;
	}
}