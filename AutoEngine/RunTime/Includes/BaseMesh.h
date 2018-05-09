#ifndef BASE_MESH_H_
#define BASE_MESH_H_
#include "BaseObject.h"

AUTO_BEGIN
class BaseMesh : public Object
{
	REGISTER_DERIVED_CLASS(BaseMesh, Object);
	DECLARE_OBJECT_SERIALIZE(BaseMesh);
public:
	BaseMesh();

};
AUTO_END
#endif //!BASE_MESH_H_
