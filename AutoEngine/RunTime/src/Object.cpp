#include "Object.h"
#include "Ambient.h"
namespace Auto3D {

Object::Object(Ambient* ambient)
	: _ambient(ambient)
{
}

Object* Object::GetSubSystem(STRING type) const
{
	return _ambient->GetSubSystem(type);
}

const STRING Object::GetAttach() 
{
	HASH_MAP(STRING, VECTOR(STRING)) objectAttachs = _ambient->GetObjectAttachs();
	for (HASH_MAP(STRING, VECTOR(STRING))::ConstIterator i = objectAttachs.Begin(); i != objectAttachs.End(); ++i)
	{
		VECTOR(STRING) t = i->second;
		if(t.Find(GetClassString())!=t.End())
			return i->first;
	}
	// return null
	return "";
}

}

