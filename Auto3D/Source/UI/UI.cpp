#include "UI.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Shader.h"

#include "../Debug/Profiler.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

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

void UI::Initialize()
{
	auto* graphics = Subsystem<Graphics>();

	assert(_graphics && _graphics->IsInitialized());

	PROFILE(InitUI);

	_graphics = graphics;




	//const size_t NUM_OBJECTS = 1;

	//float vertexData[] = {
	//	// Position             // Texcoord
	//	0.0f, 0.05f, 0.0f,      0.5f, 0.0f,
	//	0.05f, -0.05f, 0.0f,    1.0f, 1.0f,
	//	-0.05f, -0.05f, 0.0f,   0.0f, 1.0f
	//};

	//Vector<VertexElement> vertexDeclaration;
	//vertexDeclaration.Push(VertexElement(ElementType::VECTOR3, ElementSemantic::POSITION));
	//vertexDeclaration.Push(VertexElement(ElementType::VECTOR2, ElementSemantic::TEXCOORD));
	//_vb = new VertexBuffer();
	//_vb->Define(ResourceUsage::IMMUTABLE, 3, vertexDeclaration, true, vertexData);

	//Vector<VertexElement> instanceVertexDeclaration;
	//instanceVertexDeclaration.Push(VertexElement(ElementType::VECTOR3, ElementSemantic::TEXCOORD, 1, true));
	//_ivb = new VertexBuffer();
	//_ivb->Define(ResourceUsage::DYNAMIC, NUM_OBJECTS, instanceVertexDeclaration, true);

	//unsigned short indexData[] = {
	//	0,
	//	1,
	//	2
	//};

	//_ib = new IndexBuffer();
	//_ib->Define(ResourceUsage::IMMUTABLE, 3, sizeof(unsigned short), true, indexData);

	//Constant pc(ElementType::VECTOR4, "Color");
	//_pcb = new ConstantBuffer();
	//_pcb->Define(ResourceUsage::IMMUTABLE, 1, &pc);
	//_pcb->SetConstant("Color", Color::WHITE);
	//_pcb->Apply();

	//String vsCode =
	//	"#version 150\n"
	//	"\n"
	//	"in vec3 position;\n"
	//	"in vec2 texCoord;\n"
	//	"in vec3 texCoord1; // objectPosition\n"
	//	"\n"
	//	"out vec2 vTexCoord;\n"
	//	"\n"
	//	"void main()\n"
	//	"{\n"
	//	"    gl_Position = vec4(position + texCoord1, 1.0);\n"
	//	"    vTexCoord = texCoord;\n"
	//	"}\n";

	//AutoPtr<Shader> vs = new Shader();
	//vs->SetName("Test.vs");
	//vs->Define(ShaderStage::VS, vsCode);
	//_vsv = vs->CreateVariation();

	//String psCode =
	//	"#version 150\n"
	//	"\n"
	//	"layout(std140) uniform ConstantBuffer0\n"
	//	"{\n"
	//	"    vec4 color;\n"
	//	"};\n"
	//	"\n"
	//	"uniform sampler2D Texture0;\n"
	//	"in vec2 vTexCoord;\n"
	//	"out vec4 fragColor;\n"
	//	"\n"
	//	"void main()\n"
	//	"{\n"
	//	"    fragColor = color * texture(Texture0, vTexCoord);\n"
	//	"}\n";

	//AutoPtr<Shader> ps = new Shader();
	//ps->SetName("Test.ps");
	//ps->Define(ShaderStage::PS, psCode);
	//_psv = ps->CreateVariation();
}

bool UI::PrepareView()
{
	if (!IsInitialize())
		Initialize();

	return true;
}
void UI::Render(bool renderUICommand)
{
	PrepareView();
	PROFILE(RenderUI);
	
	//Texture* tex = cache->LoadResource<Texture>("Test.png");

	/*_graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH, Color(0.0f, 0.0f, 0.5f));
	_graphics->SetVertexBuffer(0, _vb);
	_graphics->SetVertexBuffer(1, _ivb);
	_graphics->SetIndexBuffer(_ib);
	_graphics->SetConstantBuffer(ShaderStage::PS, 0, _pcb);
	_graphics->SetShaders(_vsv, _psv);
	_graphics->SetTexture(0, tex);
	_graphics->SetDepthState(CompareFunc::LESS_EQUAL, true);
	_graphics->SetColorState(BlendMode::REPLACE);
	_graphics->SetRasterizerState(CullMode::BACK, FillMode::SOLID);
	_graphics->DrawIndexedInstanced(PrimitiveType::TRIANGLE_LIST, 0, 3, 0, 0, NUM_OBJECTS);*/

}

}