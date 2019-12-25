#include "CoiiliderChain2D.h"
#include "IO/MemoryBuffer.h"
#include "IO/VectorBuffer.h"
#include "Physics/PhysicsUtils.h"

namespace Auto3D {

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ACoiiliderChain2D)
	.constructor<>()
	;
}

ACoiiliderChain2D::ACoiiliderChain2D() :
	_loop(false)
{
	_fixtureDef.shape = &_chainShape;
}

ACoiiliderChain2D::~ACoiiliderChain2D() = default;


void ACoiiliderChain2D::SetLoop(bool loop)
{
	if (loop == _loop)
		return;

	_loop = loop;

	RecreateFixture();
}

void ACoiiliderChain2D::SetVertexCount(unsigned count)
{
	_vertices.Resize(count);
}

void ACoiiliderChain2D::SetVertex(unsigned index, const TVector2F& vertex)
{
	if (index >= _vertices.Size())
		return;

	_vertices[index] = vertex;

	if (index == _vertices.Size() - 1)
	{
		RecreateFixture();
	}
}

void ACoiiliderChain2D::SetVertices(const TVector<TVector2F>& vertices)
{
	_vertices = vertices;

	RecreateFixture();
}

void ACoiiliderChain2D::SetVerticesAttr(const TVector<unsigned char>& value)
{
	if (value.IsEmpty())
		return;

	TVector<TVector2F> vertices;

	FMemoryBuffer buffer(value);
	while (!buffer.IsEof())
		vertices.Push(buffer.Read<TVector2F>());

	SetVertices(vertices);
}

TVector<unsigned char> ACoiiliderChain2D::GetVerticesAttr() const
{
	FVectorBuffer ret;

	for (unsigned i = 0; i < _vertices.Size(); ++i)
		ret.Write<TVector2F>(_vertices[i]);

	return ret.Buffer();
}

void ACoiiliderChain2D::RecreateFixture()
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