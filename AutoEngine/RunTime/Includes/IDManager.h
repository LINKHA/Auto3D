#ifndef ID_MANAGER_H_
#define ID_MANAGER_H_

#define OBJECT_ID_ADD(x) 10 ## x
#define COMP_ID_ADD(x)	 11 ## x
#define SCENE_ID_ADD(x)	 100 ## x

class IDManager
{
public:
	IDManager();
	~IDManager();
};
#endif //!ID_MANAGER_H_

