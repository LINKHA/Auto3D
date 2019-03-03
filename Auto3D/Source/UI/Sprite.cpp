#include "Sprite.h"
#include "../Graphics/Texture.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::RegisterObject()
{
	RegisterFactory<Sprite>();
}

void Sprite::SetTexture(Texture* texture)
{
	_texture = texture;

	SetGeometry(texture->GetGeometry());

	//_geometry->_primitiveType = PrimitiveType::TRIANGLE_LIST;
	//_geometry->_drawStart = 0;
	//// Six vertices determine a rectangle, if instantiated this parameter becomes the number of instances
	//_geometry->_drawCount = 6;
	//_geometry->_lodDistance = false;


	//float vertexData[] = {
	//	// positions            // texture coords
	//	 1.0f,  1.0f, 0.0f,     1.0f, 1.0f, // top right
	//	 1.0f, -1.0f, 0.0f,     1.0f, 0.0f, // bottom right
	//	-1.0f, -1.0f, 0.0f,     0.0f, 0.0f, // bottom left
	//	-1.0f,  1.0f, 0.0f,     0.0f, 1.0f  // top left 
	//};

	//Vector<VertexElement> vertexDeclaration;
	//vertexDeclaration.Push(VertexElement(ElementType::VECTOR3, ElementSemantic::POSITION));
	//vertexDeclaration.Push(VertexElement(ElementType::VECTOR2, ElementSemantic::TEXCOORD));
	//SharedPtr<VertexBuffer> vb(new VertexBuffer());
	//vb->Define(ResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, vertexData);
	//_geometry->_vertexBuffer = vb;
	//
	//unsigned short indexData[] = {
	//0, 1, 3, // first triangle
	//1, 2, 3  // second triangle
	//};
	//SharedPtr<IndexBuffer> ib(new IndexBuffer());
	//ib->Define(ResourceUsage::IMMUTABLE, 6, sizeof(unsigned short), true, indexData);
	//_geometry->_indexBuffer = ib;
	
}

}