#include "Object.h"
#include "Ambient.h"
namespace Auto3D {

Object::Object(Ambient* ambient)
	: _ambient(ambient)
{
}

Object* Object::GetSubSystem(__String type) const
{
	return _ambient->GetSubSystem(type);
}

const __String Object::GetAttach() 
{
	HASH_MAP(__String, VECTOR(__String)) objectAttachs = _ambient->GetObjectAttachs();
	for (HASH_MAP(__String, VECTOR(__String))::const_iterator i = objectAttachs.begin(); i != objectAttachs.end(); ++i)
	{
		VECTOR(__String) t = i->second;
		if (find(t.begin(), t.end(), GetClassString()) != t.end())
			return i->first;
	}
	// return null
	return "";
}

}

