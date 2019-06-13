#include "OGLGraphicsUtils.h"
#include "OGLGraphics.h"
#include "../Shader.h"
#include "../ShaderVariation.h"
#include "../Texture.h"
#include "../../Base/String.h"
#include "../../Debug/Log.h"
#include "../../Math/KhMath.h"
#include <glad.h>

namespace Auto3D
{

const char* TEXTURE_2D_TO_TEXTURE_CUBE_VS = "#version 330\n"
	"layout(location = 0) in vec3 aPos;\n"
	"out vec3 WorldPos;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 view; \n"
	"void main()\n"
	"{ \n"
	"	WorldPos = aPos; \n"
	"	gl_Position = projection * view * vec4(WorldPos, 1.0); \n"
	"};\0";

const char* TEXTURE_2D_TO_TEXTURE_CUBE_FS = "#version 330\n"
	"out vec4 FragColor;\n"
	"in vec3 WorldPos;\n"
	"uniform sampler2D equirectangularMap;\n"
	"const vec2 invAtan = vec2(0.1591, 0.3183);\n"
	"vec2 SampleSphericalMap(vec3 v)\n"
	"{\n"
	"	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));\n"
	"	uv *= invAtan;\n"
	"	uv += 0.5;\n"
	"	return uv;\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = SampleSphericalMap(normalize(WorldPos));\n"
	"	vec3 color = texture(equirectangularMap, uv).rgb;\n"
	"	FragColor = vec4(color, 1.0);\n"
	"}\0";

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
//unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
bool isDirty = false;
void renderCube()
{
	// initialize (if necessary)
	if (!isDirty)
	{
		isDirty = true;
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
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


Texture* Texture2DtoTextureCube(Texture* texture2D)
{
	Shader vsShader;
	vsShader.Define(ShaderStage::VS, TEXTURE_2D_TO_TEXTURE_CUBE_VS);
	
	Shader fsShader;
	fsShader.Define(ShaderStage::PS, TEXTURE_2D_TO_TEXTURE_CUBE_FS);
	
	ShaderVariation* vsVar = vsShader.CreateVariation();
	ShaderVariation* psVar = fsShader.CreateVariation();
	vsVar->Compile();
	psVar->Compile();
	ShaderProgram shaderPro(vsVar, psVar);

	unsigned int tempFrameBuffer;
	glGenFramebuffers(1, &tempFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, tempFrameBuffer);

	Texture* textureCube = new Texture();
	textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(512, 512), texture2D->GetFormat(), 1);
	textureCube->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	textureCube->SetDataLost(false);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	Matrix4x4F captureProjection = Perspective<float>(90.0f * M_DEG, 1.0f, 0.1f, 10.0f);
	Matrix4x4F captureViews[] =
	{
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(1.0f,  0.0f,  0.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(-1.0f,  0.0f,  0.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  1.0f,  0.0f), Vector3F(0.0f,  0.0f,  1.0f)),
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f, -1.0f,  0.0f), Vector3F(0.0f,  0.0f, -1.0f)),
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  0.0f,  1.0f), Vector3F(0.0f, -1.0f,  0.0f)),
		LookAt<float>(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(0.0f,  0.0f, -1.0f), Vector3F(0.0f, -1.0f,  0.0f))
	};
	shaderPro.Link();
	
	shaderPro.SetInt("equirectangularMap", 0);
	shaderPro.SetMat4("projection", captureProjection);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2D->_texture);

	
	glViewport(0, 0, 512, 512);
	//glBindFramebuffer(GL_FRAMEBUFFER, tempFrameBuffer);
	for (unsigned int i = 0; i < 6; ++i)
	{
		shaderPro.SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureCube->_texture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return textureCube;
}

}