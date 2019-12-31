#include "ColliderChain2D.h"
#include "IO/MemoryBuffer.h"
#include "IO/VectorBuffer.h"
#include "Physics/PhysicsUtils.h"

namespace Auto3D {

AColliderChain2D::AColliderChain2D() :
	_loop(false)
{
	_fixtureDef.shape = &_chainShape;
}

AColliderChain2D::~AColliderChain2D() = default;


void AColliderChain2D::SetLoop(bool loop)
{
	if (loop == _loop)
		return;

	_loop = loop;

	RecreateFixture();
}

void AColliderChain2D::SetVertexCount(unsigned count)
{
	_vertices.Resize(count);
}

void AColliderChain2D::SetVertex(unsigned index, const TVector2F& vertex)
{
	if (index >= _vertices.Size())
		return;

	_vertices[index] = vertex;

	if (index == _vertices.Size() - 1)
	{
		RecreateFixture();
	}
}

void AColliderChain2D::SetVertices(const TVector<TVector2F>& vertices)
{
	_vertices = vertices;

	RecreateFixture();
}

void AColliderChain2D::SetVerticesAttr(const TVector<unsigned char>& value)
{
	if (value.IsEmpty())
		return;

	TVector<TVector2F> vertices;

	FMemoryBuffer buffer(value);
	while (!buffer.IsEof())
		vertices.Push(buffer.Read<TVector2F>());

	SetVertices(vertices);
}

TVector<unsigned char> AColliderChain2D::GetVerticesAttr() const
{
	FVectorBuffer ret;

	for (unsigned i = 0; i < _vertices.Size(); ++i)
		ret.Write<TVector2F>(_vertices[i]);

	return ret.Buffer();
}

void AColliderChain2D::RecreateFixture()
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