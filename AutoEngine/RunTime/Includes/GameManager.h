#ifndef GANE_MANAGER_H_
#define GANE_MANAGER_H_

#include "BaseObject.h"
AUTO_BEGIN

class GameManager : public Object
{
public:
	GameManager();
	~GameManager();
};

class LevelGameManager : public GameManager
{


};

class GlobalGameManager : public GameManager
{


};
AUTO_END



#endif // GANE_MANAGER_H_
