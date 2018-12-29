#include "LevelBehaviorObject.h"
#include "Node.h"

#include "NewDef.h"

namespace Auto3D {

LevelBehaviorObject::LevelBehaviorObject(SharedPtr<Ambient> ambient)
	:Super(ambient)
	, _levelID(INT_MAX)
{
}


LevelBehaviorObject::~LevelBehaviorObject()
{
}

const int LevelBehaviorObject::GetLevelID() const
{
	if (_levelID == INT_MAX)
	{
		WarningString("level not initialized");
	}
	return _levelID;
}

}