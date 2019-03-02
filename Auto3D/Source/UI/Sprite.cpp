#include "Sprite.h"
#include "../Graphics/Texture.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"

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

	SetGeometry(new Geometry());

	_geometry->_primitiveType = PrimitiveType::TRIANGLE_LIST;
	_geometry->_drawStart = 0;
	// Six vertices determine a rectangle, if instantiated this parameter becomes the number of instances
	_geometry->_drawCount = 6;
	_geometry->_lodDistance = false;


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
	_geometry->_vertexBuffer = new VertexBuffer();
	_geometry->_vertexBuffer->Define(ResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, vertexData);

	unsigned short indexData[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	_geometry->_indexBuffer = new IndexBuffer();
	_geometry->_indexBuffer->Define(ResourceUsage::IMMUTABLE, 6, sizeof(unsigned short), true, indexData);
}

}