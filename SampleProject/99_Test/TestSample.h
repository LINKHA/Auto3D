#pragma once
#include "../Sample.h"
using namespace Auto3D;
class TestShader;
class TestSample : public Application
{
	REGISTER_OBJECT_CLASS(TestSample, Application)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;

	float yaw = 0.0f, pitch = 20.0f;
	Camera* camera;
	SharedPtr<Scene> scene;

	/// ShaderVariation vs 
	ShaderVariation* _vsv;
	/// ShaderVariation ps
	ShaderVariation* _psv;


	/// Per-frame vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _vsFrameConstantBuffer;
	/// Per-object vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _psFrameConstantBuffer;

	SkyBoxBuffer* buffer;
	SkyBox* skyBox;
	AutoPtr<Texture> _texture;

	AutoPtr<VertexBuffer> _instanceVertexBuffer;
	/// Vertex elements for the instance vertex buffer.
	Vector<VertexElement> _instanceVertexElements;
};