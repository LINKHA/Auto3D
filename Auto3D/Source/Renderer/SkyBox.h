#pragma once
#include "GeometryNode.h"

namespace Auto3D
{

class Image;
class ShaderVariation;

class AUTO_API SkyBox : public Serializable
{
	REGISTER_OBJECT_CLASS(Serializable, OctreeNode)

public:
	/// Construct.
	SkyBox();
	/// Destruct.
	~SkyBox();

	/// Register factory and attributes.
	static void RegisterObject();
	/// Set image to skybox
	void SetImage(Image* image);

	void Init();
	void Draw(const Matrix4x4F& projection, const Matrix4x4F& view);
private:
	ShaderVariation* _cubemap;
	ShaderVariation* _equirectangularToCubemap;
	ShaderVariation* _irradianceConvolution;
	ShaderVariation* _prefilter;
	ShaderVariation* _brdfVs;
	ShaderVariation* _brdfPs;
	ShaderVariation* _backgroundVs;
	ShaderVariation* _backgroundPs;
	// pbr: setup framebuffer
	// ----------------------
	unsigned int captureFBO;
	unsigned int captureRBO;
	unsigned int hdrTexture;
	unsigned int envCubemap;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

public:
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;
	unsigned int irradianceMap;
	/// SkyBox image
	AutoPtr<Image> _image;
};

}