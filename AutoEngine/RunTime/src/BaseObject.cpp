#include "BaseObject.h"

AUTO_BEGIN
typedef AUTO_MAP(Int32, Object::RTTI) RTTIMap;
typedef AUTO_MAP(char*, SInt32) StringToClassIDMap;



static RTTIMap*									gRTTI = NULL;
static StringToClassIDMap*						gStringToClassID = NULL;
//////////////////////////////////////////////////////////////////////////
//class PPtr
//////////////////////////////////////////////////////////////////////////

template<class T> inline 
const char * PPtr<T>::GetTypeString()
{
	return T::GetPPtrTypeString();
}
template<class T> inline 
void PPtr<T>::AssignObject(const Object* o)
{
	if (o == NULL)
		m_InstanceID = 0;
	else
		m_InstanceID = o->GetInstanceID();
}

template<class T> inline
bool PPtr<T>::IsNull() const
{
	T* o = *this;
	return o == NULL;
}

template<class T> inline
bool PPtr<T>::IsValid() const
{
	T* casted = *this;
	return casted != NULL;
}

//////////////////////////////////////////////////////////////////////////
//class BaseObjeect
//////////////////////////////////////////////////////////////////////////

Object::Object()
{
	m_InstanceID = 0;
	m_ClassID = 0;
}
const std::string& Object::GetClassName()const
{
	return Object::ClassIDToString(GetClassID());
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

template<class TransferFunction>
inline void Object::Transfer(TransferFunction & transfer)
{
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

