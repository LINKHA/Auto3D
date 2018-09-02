#include "Object.h"
#include "Ambient.h"
namespace Auto3D {

Object::Object(Ambient* ambient)
	:_ambient(ambient)
{
}

Object* Object::GetSubSystem(_String type) const
{
	return _ambient->GetSubSystem(type);
}

const _String & Object::GetAttach() const
{
	AUTO_HASH_MAP(_String, _VECTOR(_String)) objectAttachs = _ambient->GetObjectAttachs();
	for (AUTO_HASH_MAP(_String, _VECTOR(_String))::const_iterator i = objectAttachs.begin(); i != objectAttachs.end(); ++i)
	{
		_VECTOR(_String) t = i->second;
		if (find(t.begin(), t.end(), GetClassStringStatic()) != t.end())
			return i->first;
	}
	// return null
	return "";
}

}

