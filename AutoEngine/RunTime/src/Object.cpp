#include "Object.h"
#include "Ambient.h"
AUTO_BEGIN
typedef AUTO_MAP(Int32, Object::RTTI) RTTIMap;
typedef AUTO_MAP(char*, SInt32) StringToClassIDMap;



static RTTIMap*									gRTTI = NULL;
static StringToClassIDMap*						gStringToClassID = NULL;
//////////////////////////////////////////////////////////////////////////
//class BaseObjeect
//////////////////////////////////////////////////////////////////////////

Object::Object(Ambient* ambient)
	:_ambient(ambient)
{
	_instanceID = 0;
	_classID = 0;
}
const std::string& Object::GetClassName()const
{
	return Object::ClassIDToString(GetClassIDVirtual());
}
const std::string& Object::ClassIDToString(int ID)
{
	static std::string emptyString;
	RTTIMap::iterator it = gRTTI->find(ID);
	if (it == gRTTI->end())
		return emptyString;
	else
		return it->second.className;
}


int Object::StringToClassID(const std::string & classString)
{
	StringToClassIDMap::iterator i;
	i = gStringToClassID->find(const_cast<char*> (classString.c_str()));
	if (i == gStringToClassID->end())
		return -1;
	else
		return i->second;
}

int Object::StringToClassID(const char * classString)
{
	StringToClassIDMap::iterator i;
	i = gStringToClassID->find(const_cast<char*> (classString));
	if (i == gStringToClassID->end())
		return -1;
	else
		return i->second;
}
AUTO_END

