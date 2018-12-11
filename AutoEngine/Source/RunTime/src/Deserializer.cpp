#include "Deserializer.h"
#include "NewDef.h"


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

const STRING& Deserializer::GetName()const
{
	return STRING();
}

unsigned Deserializer::GetChecksum()
{
	return 0;
}

long long Deserializer::ReadInt64()
{
	long long ret;
	Read(&ret, sizeof ret);
	return ret;
}

int Deserializer::ReadInt()
{
	int ret;
	Read(&ret, sizeof ret);
	return ret;
}

short Deserializer::ReadShort()
{
	short ret;
	Read(&ret, sizeof ret);
	return ret;
}

signed char Deserializer::ReadByte()
{
	signed char ret;
	Read(&ret, sizeof ret);
	return ret;
}

unsigned long long Deserializer::ReadUInt64()
{
	unsigned long long ret;
	Read(&ret, sizeof ret);
	return ret;
}

unsigned Deserializer::ReadUInt()
{
	unsigned ret;
	Read(&ret, sizeof ret);
	return ret;
}

unsigned short Deserializer::ReadUShort()
{
	unsigned short ret;
	Read(&ret, sizeof ret);
	return ret;
}

unsigned char Deserializer::ReadUByte()
{
	unsigned char ret;
	Read(&ret, sizeof ret);
	return ret;
}

bool Deserializer::ReadBool()
{
	return ReadUByte() != 0;
}

float Deserializer::ReadFloat()
{
	float ret;
	Read(&ret, sizeof ret);
	return ret;
}

double Deserializer::ReadDouble()
{
	double ret;
	Read(&ret, sizeof ret);
	return ret;
}

STRING Deserializer::ReadFileID()
{
	STRING ret;
	ret.Resize(4);
	Read(&ret[0], 4);
	return ret;
}

STRING Deserializer::ReadLine()
{
	STRING ret;

	while (!IsEof())
	{
		char c = ReadByte();
		if (c == 10)
			break;
		if (c == 13)
		{
			// Peek next char to see if it's 10, and skip it too
			if (!IsEof())
			{
				char next = ReadByte();
				if (next != 10)
					Seek(_position - 1);
			}
			break;
		}
		ret += c;
	}

	return ret;
}

}