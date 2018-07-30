#pragma once
#include "Texture3D.h"
#include "Singleton.h"
AUTO_BEGIN

class SkyBox : public Texture3D
{
	REGISTER_DERIVED_CLASS(SkyBox, Texture3D);
	DECLARE_OBJECT_SERIALIZE(SkyBox);
public:
	explicit SkyBox(Ambient* ambient);

	void Start()override;
	void Draw()override;
	unsigned int GetTexture() { return _cubemapTexture; }
private:
	unsigned int _cubemapTexture;

	Shader _shader;
	unsigned int _skyboxVAO;
	unsigned int _skyboxVBO;
};

struct SkyManager : public Singleton<SkyManager>
{
	void AddSkyBox(SkyBox* skybox) { _skybox = skybox; }
	SkyBox* GetSkyBox() { return _skybox; }
private:
	SkyBox* _skybox;
};
AUTO_END

