#include "CoiiliderChain2D.h"
#include "IO/MemoryBuffer.h"
#include "IO/VectorBuffer.h"
#include "Physics/PhysicsUtils.h"

namespace Auto3D {

CoiiliderChain2D::CoiiliderChain2D() :
	_loop(false)
{
	_fixtureDef.shape = &_chainShape;
}

CoiiliderChain2D::~CoiiliderChain2D() = default;

void CoiiliderChain2D::RegisterObject()
{
	RegisterFactory<CoiiliderChain2D>();
}


void CoiiliderChain2D::SetLoop(bool loop)
{
	if (loop == _loop)
		return;

	_loop = loop;

	RecreateFixture();
}

void CoiiliderChain2D::SetVertexCount(unsigned count)
{
	_vertices.Resize(count);
}

void CoiiliderChain2D::SetVertex(unsigned index, const Vector2F& vertex)
{
	if (index >= _vertices.Size())
		return;

	_vertices[index] = vertex;

	if (index == _vertices.Size() - 1)
	{
		RecreateFixture();
	}
}

void CoiiliderChain2D::SetVertices(const TVector<Vector2F>& vertices)
{
	_vertices = vertices;

	RecreateFixture();
}

void CoiiliderChain2D::SetVerticesAttr(const TVector<unsigned char>& value)
{
	if (value.IsEmpty())
		return;

	TVector<Vector2F> vertices;

	MemoryBuffer buffer(value);
	while (!buffer.IsEof())
		vertices.Push(buffer.Read<Vector2F>());

	SetVertices(vertices);
}

TVector<unsigned char> CoiiliderChain2D::GetVerticesAttr() const
{
	VectorBuffer ret;

	for (unsigned i = 0; i < _vertices.Size(); ++i)
		ret.Write<Vector2F>(_vertices[i]);

	return ret.Buffer();
}

void CoiiliderChain2D::RecreateFixture()
{
	ReleaseFixture();

	TVector<b2Vec2> b2Vertices;
	unsigned count = _vertices.Size();
	b2Vertices.Resize(count);

	for (unsigned i = 0; i < count; ++i)
		b2Vertices[i] = ToB2Vector2(_vertices[i]);

	_chainShape.Clear();
	if (_loop)
		_chainShape.CreateLoop(&b2Vertices[0], count);
	else
		_chainShape.CreateChain(&b2Vertices[0], count);

	CreateFixture();
}

}