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
	DO_CLASS_ID(TimeManager, 4)

	DO_CLASS_ID(Node, 8)
	DO_CLASS_ID(Sprite, 9)
	DO_CLASS_ID(BaseMesh, 10)
	DO_CLASS_ID(Mesh, 11)
	DO_CLASS_ID(BaseTexture, 12)
	DO_CLASS_ID(Texture2D, 13)
	DO_CLASS_ID(BaseLight, 14)
	DO_CLASS_ID(AmbientLighting, 15)
	DO_CLASS_ID(Component,16)
	
	CLASS_ID_Count
};



#endif // !CLASS_ID_H_
