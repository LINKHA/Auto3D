#include "Cursor.h"
AUTO_BEGIN
template<> Cursor* Singleton<Cursor>::m_instance = nullptr;

Cursor::Cursor()
{
}


Cursor::~Cursor()
{
}
AUTO_END