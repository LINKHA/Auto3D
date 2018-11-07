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
	for (HASH_MAP(STRING, VECTOR(STRING))::const_iterator i = objectAttachs.begin(); i != objectAttachs.end(); ++i)
	{
		VECTOR(STRING) t = i->second;
		if (find(t.begin(), t.end(), GetClassString()) != t.end())
			return i->first;
	}
	// return null
	return "";
}

}

