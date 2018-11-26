#include "Deserializer.h"


namespace Auto3D {


Deserializer::Deserializer()
	: _position(0)
	, _size(0)
{

}

Deserializer::Deserializer(unsigned size)
	: _position(0)
	, _size(size)
{

}

Deserializer::~Deserializer() = default;

const STRING& Deserializer::GetName() const
{
	return STRING();
}

unsigned Deserializer::GetChecksum()
{
	return 0;
}
}