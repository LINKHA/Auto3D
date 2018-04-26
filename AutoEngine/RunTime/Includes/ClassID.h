#ifndef CLASS_ID_H_
#define CLASS_ID_H_
#define ClassID(x)		  CLASS_##x
#define DO_CLASS_ID(x,ID) ClassID(x) = ID,
enum ClassIDType
{
	DO_CLASS_ID(Default,-1)
	DO_CLASS_ID(Object, 0)
	DO_CLASS_ID(GameManager,1)
	DO_CLASS_ID(LevelGameManager, 2)
	DO_CLASS_ID(GlobalGameManager, 3)


	CLASS_ID_Count
};



#endif // !CLASS_ID_H_
