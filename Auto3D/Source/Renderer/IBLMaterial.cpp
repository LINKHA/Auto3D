#include "IBLMaterial.h"

#include "../Engine/ModuleManager.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"
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


void renderCube()
{
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

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

void renderQuad()
{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
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

tShader irradianceShader;
tShader prefilterShader;
tShader brdfShader;


unsigned int irradianceBuffer;
unsigned int prefilterBuffer;
unsigned int brdfLUTBuffer;

IBLMaterial::IBLMaterial():
	_mapSize(512)
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
	auto cache = ModuleManager::Get().CacheModule();
	String path = cache->ResourceDirs()[0];

	irradianceShader = tShader((path + "Shader/IBL/Cubemap.vs").CString(), (path + "Shader/IBL/IrradianceConvolution.fs").CString());
	prefilterShader = tShader((path + "Shader/IBL/Cubemap.vs").CString(), (path + "Shader/IBL/Prefilter.fs").CString());
	brdfShader = tShader((path + "Shader/IBL/Brdf.vs").CString(), (path + "Shader/IBL/Brdf.fs").CString());

	// pbr: setup framebuffer
   // ----------------------

	unsigned int captureFBO;
	unsigned int captureRBO;
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

	glGenTextures(1, &irradianceBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceBuffer);
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceBuffer, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region create prefilter map

	// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	// --------------------------------------------------------------------------------

	glGenTextures(1, &prefilterBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterBuffer);
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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterBuffer, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region create prefilter map
	// pbr: generate a 2D LUT from the BRDF equations used.
	// ----------------------------------------------------

	glGenTextures(1, &brdfLUTBuffer);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, _mapSize, _mapSize, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _mapSize, _mapSize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTBuffer, 0);

	glViewport(0, 0, _mapSize, _mapSize);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	SharedPtr<Texture> irradianceMap(new Texture());
	SharedPtr<Texture> prefilterMap(new Texture());
	SharedPtr<Texture> brdfLUTTexture(new Texture());
	irradianceMap->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(_mapSize, _mapSize), ImageFormat::D24S8, 1);
	irradianceMap->_texture = irradianceBuffer;

	prefilterMap->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(_mapSize, _mapSize), ImageFormat::D24S8, 1);
	prefilterMap->_texture = prefilterBuffer;
	
	brdfLUTTexture->_texture = brdfLUTBuffer;

	SetTexture(5, irradianceMap);
	SetTexture(6, prefilterMap);
	SetTexture(7, brdfLUTTexture);


}

}