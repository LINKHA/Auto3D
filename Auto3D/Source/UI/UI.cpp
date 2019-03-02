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

#pragma warnning
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"

namespace Auto3D
{
	const size_t NUM_OBJECTS = 1;
	Texture* tex;

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
	//_graphics->SetVertexBuffer(0, _vb);
	//_graphics->SetVertexBuffer(1, _ivb);
	//_graphics->SetIndexBuffer(_ib);
	_graphics->SetConstantBuffer(ShaderStage::PS, 0, _pcb);

	_graphics->SetShaders(_vsv, _psv);
	_graphics->SetTexture(0, tex);
	_graphics->SetDepthState(CompareFunc::LESS_EQUAL, true);
	_graphics->SetColorState(BlendMode::REPLACE);
	_graphics->SetRasterizerState(CullMode::BACK, FillMode::SOLID);

	RenderBatches();

	//Vector3F instanceData[NUM_OBJECTS];
	//for (size_t i = 0; i < NUM_OBJECTS; ++i)
	//{
	//	//instanceData[i] = Vector3F(Random() * 2.0f - 1.0f, Random() * 2.0f - 1.0f, 0.0f);
	//	instanceData[i] = Vector3F();
	//}

	//_ivb->SetData(0, NUM_OBJECTS, instanceData);

	//_graphics->SetVertexBuffer(0, _vb);
	//_graphics->SetVertexBuffer(1, _ivb);
	//_graphics->SetIndexBuffer(_ib);
	//_graphics->SetConstantBuffer(ShaderStage::PS, 0, _pcb);

	//_graphics->SetShaders(_vsv, _psv);
	//_graphics->SetTexture(0, tex);
	//_graphics->SetDepthState(CompareFunc::LESS_EQUAL, true);
	//_graphics->SetColorState(BlendMode::REPLACE);
	//_graphics->SetRasterizerState(CullMode::BACK, FillMode::SOLID);
	//_graphics->DrawIndexedInstanced(PrimitiveType::TRIANGLE_LIST, 0, 6, 0, 0, NUM_OBJECTS);
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
		if(it->second->TestFlag(UNF_GEOMETRY))
			_geometryNode.Push(static_cast<UIGeometryNode*>(it->second));
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


	float vertexData[] = {
		// positions            // texture coords
		 1.0f,  1.0f, 0.0f,     1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,     1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,     0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,     0.0f, 1.0f  // top left 
	};

	Vector<VertexElement> vertexDeclaration;
	vertexDeclaration.Push(VertexElement(ElementType::VECTOR3, ElementSemantic::POSITION));
	vertexDeclaration.Push(VertexElement(ElementType::VECTOR2, ElementSemantic::TEXCOORD));
	_vb = new VertexBuffer();
	_vb->Define(ResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, vertexData);


	Vector<VertexElement> instanceVertexDeclaration;
	instanceVertexDeclaration.Push(VertexElement(ElementType::VECTOR3, ElementSemantic::TEXCOORD, 1, true));
	_ivb = new VertexBuffer();
	_ivb->Define(ResourceUsage::DYNAMIC, NUM_OBJECTS, instanceVertexDeclaration, true);

	unsigned short indexData[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	_ib = new IndexBuffer();
	_ib->Define(ResourceUsage::IMMUTABLE, 6, sizeof(unsigned short), true, indexData);

	Constant pc(ElementType::VECTOR4, "Color");
	_pcb = new ConstantBuffer();
	_pcb->Define(ResourceUsage::IMMUTABLE, 1, &pc);
	_pcb->SetConstant("Color", Color::WHITE);
	_pcb->Apply();

	String vsCode =
		"#version 150\n"
		"\n"
		"in vec3 position;\n"
		"in vec2 texCoord;\n"
		"in vec3 texCoord1; // objectPosition\n"
		"\n"
		"out vec2 vTexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position + texCoord1, 1.0);\n"
		"    vTexCoord = texCoord;\n"
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

	tex = Subsystem<ResourceCache>()->LoadResource<Texture>("Test.png");

}

void UI::RenderBatches(const Vector<UIBatch>& batches, UICamera* camera)
{

	for (auto it = batches.Begin(); it != batches.End();)
	{
		const UIBatch& batch = *it;
		bool instanced = batch._type == GeometryType::INSTANCED;

		Geometry* geometry = batch._geometry;
		// Set vertex / index buffers and draw
		if (instanced)
			geometry->DrawInstanced(_graphics, 0, batch._instanceCount);
		else
			geometry->Draw(_graphics);
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