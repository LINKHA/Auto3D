#include "BaseObject.h"

AUTO_BEGIN


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


AUTO_END

