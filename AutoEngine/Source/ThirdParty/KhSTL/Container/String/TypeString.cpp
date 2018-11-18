#include "TypeString.h"

namespace KhSTL {

const tString tString::EMPTY;

std::istream& operator >>(std::istream &in, tString& rhs)
{
	in >> rhs._buffer;
	return in;
}

std::ostream& operator <<(std::ostream& out, tString& rhs)
{
	out << rhs._buffer;
	return out;
}

}