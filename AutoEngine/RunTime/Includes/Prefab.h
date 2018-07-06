#ifndef PRE_FAB_H_
#define PRE_FAB_H_
#include "BaseObject.h"
#include "GameObject.h"
AUTO_BEGIN
class Prefab : public Object
{
	REGISTER_DERIVED_CLASS(Prefab, Object);
	DECLARE_OBJECT_SERIALIZE(Prefab);
public:
	Prefab(GameObject * gameObject);
	Prefab(GameObject& gameObject);
private:
	GameObject * m_GameObject;
};

AUTO_END
#endif // !PRE_FAB_H_