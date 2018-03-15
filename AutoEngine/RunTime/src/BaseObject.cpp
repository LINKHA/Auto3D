#include "BaseObject.h"

AUTO_BEGIN

//class PPtr
template<class T>
inline void PPtr<T>::AssignObject(const BaseObject* o)
{
	if (o == NULL)
		m_InstanceID = 0;
	else
		m_InstanceID = o->GetInstanceID();
}




BaseObject::BaseObject()
{}
BaseObject::~BaseObject()
{}
AUTO_END

