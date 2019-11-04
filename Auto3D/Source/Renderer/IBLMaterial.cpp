#include "IBLMaterial.h"

#include "../Engine/ModuleManager.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Texture.h"

#include <glad.h>
#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace Auto3D
{

	class tShader
	{
	public:
		unsigned int ID;
		tShader() {}
		// constructor generates the shader on the fly
		// ------------------------------------------------------------------------
		tShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
		{
			// 1. retrieve the vertex/fragment source code from filePath
			std::string vertexCode;
			std::string fragmentCode;
			std::string geometryCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			std::ifstream gShaderFile;
			// ensure ifstream objects can throw exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;
				// read file's buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert stream into string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
				// if geometry shader path is present, also load a geometry shader
				if (geometryPath != nullptr)
				{
					gShaderFile.open(geometryPath);
					std::stringstream gShaderStream;
					gShaderStream << gShaderFile.rdbuf();
					gShaderFile.close();
					geometryCode = gShaderStream.str();
				}
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}
			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			// 2. compile shaders
			unsigned int vertex, fragment;
			// vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, "VERTEX");
			// fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, "FRAGMENT");
			// if geometry shader is given, compile geometry shader
			unsigned int geometry;
			if (geometryPath != nullptr)
			{
				const char* gShaderCode = geometryCode.c_str();
				geometry = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometry, 1, &gShaderCode, NULL);
				glCompileShader(geometry);
				checkCompileErrors(geometry, "GEOMETRY");
			}
			// shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			if (geometryPath != nullptr)
				glAttachShader(ID, geometry);
			glLinkProgram(ID);
			checkCompileErrors(ID, "PROGRAM");
			// delete the shaders as they're linked into our program now and no longer necessery
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			if (geometryPath != nullptr)
				glDeleteShader(geometry);

		}
		// activate the shader
		// ------------------------------------------------------------------------
		void use()
		{
			glUseProgram(ID);
		}
		// utility uniform functions
		// ------------------------------------------------------------------------
		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		// ------------------------------------------------------------------------
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}

		void setVec3(const std::string& name, const Vector3F& value) const
		{
			glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value.Data());
		}
		void setVec3(const std::string& name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
		}
		// ------------------------------------------------------------------------
		void setMat4(const std::string& name, const Matrix4x4F& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.Data());
		}

	private:
		// utility function for checking shader compilation/linking errors.
		// ------------------------------------------------------------------------
		void checkCompileErrors(GLuint shader, std::string type)
		{
			GLint success;
			GLchar infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}
	};


// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		Vector<Vector3F> positions;
		Vector<Vector2F> uv;
		Vector<Vector3F> normals;
		Vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.Push(Vector3F(xPos, yPos, zPos));
				uv.Push(Vector2F(xSegment, ySegment));
				normals.Push(Vector3F(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.Push(y * (X_SEGMENTS + 1) + x);
					indices.Push((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.Push((y + 1) * (X_SEGMENTS + 1) + x);
					indices.Push(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.Size();

		Vector<float> data;
		for (int i = 0; i < positions.Size(); ++i)
		{
			data.Push(positions[i]._x);
			data.Push(positions[i]._y);
			data.Push(positions[i]._z);
			if (uv.Size() > 0)
			{
				data.Push(uv[i]._x);
				data.Push(uv[i]._y);
			}
			if (normals.Size() > 0)
			{
				data.Push(normals[i]._x);
				data.Push(normals[i]._y);
				data.Push(normals[i]._z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.Size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
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
		//glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		//glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);
	}
	// render Cube
	//glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		//glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		//glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	//glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
tShader pbrShader;
tShader equirectangularToCubemapShader;
tShader irradianceShader;
tShader prefilterShader;
tShader brdfShader;
tShader backgroundShader;

unsigned int goldAlbedoMap;
unsigned int goldNormalMap;
unsigned int goldMetallicMap;
unsigned int goldRoughnessMap;
unsigned int goldAOMap;


unsigned int captureFBO;
unsigned int captureRBO;

unsigned int envCubemap;
unsigned int irradianceMap;
unsigned int prefilterMap;
unsigned int brdfLUTTexture;

IBLMaterial::IBLMaterial()
{
	
}

IBLMaterial::~IBLMaterial()
{

}

void IBLMaterial::RegisterObject()
{
	RegisterFactory<IBLMaterial>();
}

void IBLMaterial::CreatePass(Camera* camera)
{

	camera->SetPosition(Vector3F(0, 0, 3));
		


	while (1)
	{
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Matrix4x4F model;
		Matrix4x4F view = camera->GetViewMatrix();
		view = Matrix4x4F(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,-3,1
		);

		// render scene, supplying the convoluted irradiance map to the final shader.
	  // ------------------------------------------------------------------------------------------
		pbrShader.use();

		//Matrix4x4F view = camera->GetViewMatrix();
		pbrShader.setMat4("view", view);
		pbrShader.setVec3("camPos", camera->GetPosition());

		// bind pre-computed IBL data
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);


		// gold
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, goldNormalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, goldAOMap);


		model = Matrix4x4F();
		pbrShader.setMat4("model", model);
		renderSphere();

		ModuleManager::Get().GraphicsModule()->Present();
	}
}

void IBLMaterial::SetIrradiance()
{

}

void IBLMaterial::SetPrefilter()
{

}

void IBLMaterial::SetBrdfLut()
{

}

void IBLMaterial::SetAAA(Texture* iblCube)
{
	// configure global opengl state
// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	// build and compile shaders
// -------------------------
	pbrShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.pbr.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.pbr.fs");
	equirectangularToCubemapShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.cubemap.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.equirectangular_to_cubemap.fs");
	irradianceShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.cubemap.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.irradiance_convolution.fs");
	prefilterShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.cubemap.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.prefilter.fs");
	brdfShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.brdf.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.brdf.fs");
	backgroundShader = tShader("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.background.vs", "E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/2.2.2.background.fs");

	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);
	pbrShader.setInt("albedoMap", 3);
	pbrShader.setInt("normalMap", 4);
	pbrShader.setInt("metallicMap", 5);
	pbrShader.setInt("roughnessMap", 6);
	pbrShader.setInt("aoMap", 7);

	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	goldAlbedoMap = loadTexture("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/gold/albedo.png");
	goldNormalMap = loadTexture("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/gold/normal.png");
	goldMetallicMap = loadTexture("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/gold/metallic.png");
	goldRoughnessMap = loadTexture("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/gold/roughness.png");
	goldAOMap = loadTexture("E:/Project/MyProject/Auto3D/Auto3D/Source/Renderer/gold/ao.png");

	// pbr: setup framebuffer
   // ----------------------

	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	Matrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
	Matrix4x4F captureViews[] =
	{
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(1.0f,  0.0f,  0.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(-1.0f,  0.0f,  0.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  1.0f,  0.0f), Vector3F(0.0f,  0.0f,  1.0f)),
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, -1.0f,  0.0f), Vector3F(0.0f,  0.0f, -1.0f)),
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  0.0f,  1.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  0.0f, -1.0f), Vector3F(0.0f, -1.0f,  0.0f))
	};

#pragma endregion HDR»·¾³

#pragma region create irradiance map
	// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	// --------------------------------------------------------------------------------

	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblCube->GetGLTexture());

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region create prefilter map

	// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	// --------------------------------------------------------------------------------

	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	// ----------------------------------------------------------------------------------------------------
	prefilterShader.use();
	prefilterShader.setInt("environmentMap", 0);
	prefilterShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblCube->GetGLTexture());

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region create prefilter map
	// pbr: generate a 2D LUT from the BRDF equations used.
	// ----------------------------------------------------

	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	Matrix4x4F projection = Perspective(45.0f, (float)1024 / (float)768, 0.1f, 100.0f);
	pbrShader.use();
	pbrShader.setMat4("projection", projection);

	// then before rendering, configure the viewport to the original framebuffer's screen dimensions
	glViewport(0, 0, 1024, 768);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture* tex1 = new Texture();
	Texture* tex2 = new Texture();
	Texture* tex3 = new Texture();
	tex1->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(512, 512), ImageFormat::D24S8, 1);
	tex1->_texture = irradianceMap;

	tex2->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(512, 512), ImageFormat::D24S8, 1);
	tex2->_texture = prefilterMap;
	
	tex3->_texture = brdfLUTTexture;

	SetTexture(5, tex1);
	SetTexture(6, tex2);
	SetTexture(7, tex3);


}

}