#include "UI.h"
#include "Canvas.h"
#include "Sprite.h"
#include "UICamera.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Shader.h"

#include <stb_image.h>

#include "../Debug/Profiler.h"
#include "../Debug/DebugNew.h"


namespace Auto3D
{

/// Parameter indices in constant buffers used by high-level rendering.
static const size_t VS_FRAME_VIEW_MATRIX = 0;
static const size_t VS_FRAME_PROJECTION_MATRIX = 1;
static const size_t VS_FRAME_VIEWPROJ_MATRIX = 2;
static const size_t VS_FRAME_DEPTH_PARAMETERS = 3;
static const size_t VS_OBJECT_WORLD_MATRIX = 0;

UI::UI() :
	_graphics(nullptr),
	_initialized(false),
	_uiRendered(false)
{
	RegisterSubsystem(this);

}

UI::~UI()
{
	RemoveSubsystem(this);
}

void UI::Render(Canvas* scene, UICamera* camera)
{
	PrepareView(scene, camera);

	PROFILE(RenderUI);

	// Set per-frame values to the frame constant buffers
	Matrix3x4F viewMatrix = camera->GetViewMatrix();
	Matrix4x4F projectionMatrix = camera->GetProjectionMatrix();
	Matrix4x4F viewProjMatrix = projectionMatrix * viewMatrix;
	Vector4F depthParameters(Vector4F::ZERO);
	depthParameters._x = camera->GetNearClip();
	depthParameters._y = camera->GetFarClip();
	if (camera->IsOrthographic())
	{
#ifdef USE_OPENGL
		depthParameters._z = 0.5f;
		depthParameters._w = 0.5f;
#else
		depthParameters._z = 1.0f;
#endif
	}
	else
		depthParameters._w = 1.0f / _camera->GetFarClip();

	_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEW_MATRIX, viewMatrix);
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_PROJECTION_MATRIX, projectionMatrix);
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEWPROJ_MATRIX, viewProjMatrix);
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_DEPTH_PARAMETERS, depthParameters);


	_vsFrameConstantBuffer->Apply();

	


	_psFrameConstantBuffer->SetConstant("Color", Color::WHITE);
	_psFrameConstantBuffer->Apply();

	
	_graphics->SetDepthState(CompareFunc::LESS_EQUAL, true);
	_graphics->SetColorState(BlendMode::REPLACE);
	_graphics->SetRasterizerState(CullMode::BACK, FillMode::SOLID);

	RenderBatches();
}

bool UI::PrepareView(Canvas* canvas,UICamera* camera)
{
	if (!IsInitialized())
		Initialize();
	if (!CollectUIObjects(canvas, camera))
		return false;
	

	CollectUIBatches();
	return true;
}

bool UI::CollectUIObjects(Canvas* canvas, UICamera* camera)
{
	PROFILE(CollectUIObjects);

	_geometryNode.Clear();
	_batchQueue.Clear();

	_canvas = canvas;
	_camera = camera;
	//Classify UI nodes to remove nodes that are not in the view
	//\note TEMP Temporarily all join
	for (auto it = canvas->GetAllUINode().Begin(); it != canvas->GetAllUINode().End(); it++)
	{
		if (it->second->TestFlag(UNF_GEOMETRY))
		{
			_geometryNode.Push(static_cast<UIGeometryNode*>(it->second));
		}
	}

	return true;
}

void UI::CollectUIBatches()
{
	PROFILE(CollectUIBatches);

	for (auto it = _geometryNode.Begin(); it != _geometryNode.End(); it++)
	{
		
		UIGeometryNode* node = *it;

		UIBatch newBatch;

		newBatch._type = node->GetGeometryType();
		newBatch._geometry = node->GetGeometry();
		newBatch._worldMatrix = &node->GetWorldTransform();

		newBatch.CalculateSortKey();
		
		_batchQueue._batches.Push(newBatch);
	}
	_batchQueue.Sort();
}

void UI::RenderBatches()
{
	PROFILE(RenderUIBatches);
	RenderBatches(_batchQueue._batches, _camera);
}

void UI::Initialize()
{
	auto* graphics = Subsystem<Graphics>();

	assert(!_graphics && !IsInitialized());
	// Invert the UI image to make it display correctly
	stbi_set_flip_vertically_on_load(true);

	PROFILE(InitUI);

	_graphics = graphics;
	_initialized = true;


	Vector<Constant> constants;
	_vsFrameConstantBuffer = new ConstantBuffer();

	constants.Push(Constant(ElementType::MATRIX3X4, "viewMatrix"));
	constants.Push(Constant(ElementType::MATRIX4, "projectionMatrix"));
	constants.Push(Constant(ElementType::MATRIX4, "viewProjMatrix"));
	constants.Push(Constant(ElementType::VECTOR4, "depthParameters"));
	_vsFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);
	//_vsFrameConstantBuffer->Apply();

	_vsObjectConstantBuffer = new ConstantBuffer();
	constants.Clear();
	constants.Push(Constant(ElementType::MATRIX3X4, "worldMatrix"));
	_vsObjectConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

	_psFrameConstantBuffer = new ConstantBuffer();
	constants.Clear();
	constants.Push(Constant(ElementType::VECTOR4, "Color"));
	_psFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);
	


	String vsCode =
		"#version 150\n"
		"\n"
		"in vec3 position;\n"
		"in vec2 texCoord;\n"
		"in vec3 texCoord1; // objectPosition\n"
		"layout(std140) uniform PerFrameVS0\n"
		"{\n"
		"mat3x4 viewMatrix;\n"
		"mat4 projectionMatrix;\n"
		"mat4 viewProjMatrix;\n"
		"vec4 depthParameters;\n"
		"};\n"
		"layout(std140) uniform PerObjectVS1\n"
		"{\n"
		"mat3x4 worldMatrix;\n"
		"};\n"
		"out vec4 vWorldPos;\n"
		"\n"
		"out vec2 vTexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vWorldPos.xyz = vec4(position, 1.0) * worldMatrix;\n"
		"	gl_Position = vec4(vWorldPos.xyz, 1.0) * viewProjMatrix;\n"
		"   vTexCoord = texCoord;\n"
		"}\n";

	vs = new Shader();
	vs->SetName("Test.vs");
	vs->Define(ShaderStage::VS, vsCode);
	_vsv = vs->CreateVariation();

	String psCode =
		"#version 150\n"
		"\n"
		"layout(std140) uniform ConstantBuffer0\n"
		"{\n"
		"    vec4 color;\n"
		"};\n"
		"\n"
		"uniform sampler2D Texture0;\n"
		"in vec2 vTexCoord;\n"
		"out vec4 fragColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    fragColor = color * texture(Texture0, vTexCoord);\n"
		"}\n";

	ps = new Shader();
	ps->SetName("Test.ps");
	ps->Define(ShaderStage::PS, psCode);
	_psv = ps->CreateVariation();

}

void UI::RenderBatches(const Vector<UIBatch>& batches, UICamera* camera)
{

	for (auto it = batches.Begin(); it != batches.End();)
	{
		const UIBatch& batch = *it;
		bool instanced = batch._type == GeometryType::INSTANCED;

		_vsObjectConstantBuffer->SetConstant(VS_OBJECT_WORLD_MATRIX, batch._worldMatrix);
		_vsObjectConstantBuffer->Apply();

		_graphics->SetConstantBuffer(ShaderStage::VS, UIConstantBuffer::FRAME, _vsFrameConstantBuffer);
		_graphics->SetConstantBuffer(ShaderStage::PS, UIConstantBuffer::FRAME, _psFrameConstantBuffer);

		_graphics->SetConstantBuffer(ShaderStage::VS, UIConstantBuffer::OBJECT, _vsObjectConstantBuffer);


		_graphics->SetShaders(_vsv, _psv);

		Geometry* geometry = batch._geometry;
		// Set vertex / index buffers and draw
		if (instanced)
			geometry->DrawInstanced(_graphics, 0, batch._instanceCount);
		else
			geometry->Draw(_graphics);

		// Advance. If instanced, skip over the batches that were converted
		it += instanced ? batch._instanceCount : 1;
	}
}

void RegisterUILibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	Canvas::RegisterObject();
	Sprite::RegisterObject();
	UISpatialNode::RegisterObject();
	UICamera::RegisterObject();
	UINode::RegisterObject();
	UIGeometryNode::RegisterObject();
}

}